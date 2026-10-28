/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "Supervisor.hpp"

#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <string>

namespace supervisor {

namespace {

volatile std::sig_atomic_t g_terminate_signal{0};
volatile pid_t g_child_pid{0};

void forward_signal(int sig) {
	g_terminate_signal = sig;
	const pid_t child{g_child_pid};
	if (child > 0) {
		(void)::kill(child, sig);  // async-signal-safe
	}
}

void install_handlers(struct sigaction& old_term, struct sigaction& old_int, struct sigaction& old_hup) {
	struct sigaction action{};
	action.sa_handler = forward_signal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;  // no SA_RESTART: waitpid()/nanosleep() must be interruptible
	(void)sigaction(SIGTERM, &action, &old_term);
	(void)sigaction(SIGINT, &action, &old_int);
	(void)sigaction(SIGHUP, &action, &old_hup);
}

void restore_handlers(const struct sigaction& old_term,
					  const struct sigaction& old_int,
					  const struct sigaction& old_hup) {
	(void)sigaction(SIGTERM, &old_term, nullptr);
	(void)sigaction(SIGINT, &old_int, nullptr);
	(void)sigaction(SIGHUP, &old_hup, nullptr);
}

void log_line(const std::string& text) {
	std::fprintf(stderr, "[supervisor] %s\n", text.c_str());
	std::fflush(stderr);
}

// sleeps for `duration` but returns early (true) if a termination signal arrived
bool interruptible_sleep(std::chrono::milliseconds duration) {
	auto remaining{duration};
	while (remaining.count() > 0) {
		if (g_terminate_signal != 0) {
			return true;
		}
		const auto slice{std::min(remaining, std::chrono::milliseconds{200})};
		struct timespec ts{};
		ts.tv_sec = static_cast<time_t>(slice.count() / 1000);
		ts.tv_nsec = static_cast<long>((slice.count() % 1000) * 1'000'000);
		(void)::nanosleep(&ts, nullptr);
		remaining -= slice;
	}
	return g_terminate_signal != 0;
}

[[noreturn]] void run_child(const std::function<int()>& worker, pid_t supervisor_pid) {
	// default signal handling in the worker; die with the supervisor
	(void)std::signal(SIGTERM, SIG_DFL);
	(void)std::signal(SIGINT, SIG_DFL);
	(void)std::signal(SIGHUP, SIG_DFL);
	(void)::prctl(PR_SET_PDEATHSIG, SIGTERM);
	if (::getppid() != supervisor_pid) {
		std::_Exit(EXIT_FAILURE);  // supervisor already gone (PDEATHSIG race)
	}

	int code{EXIT_FAILURE};
	try {
		code = worker();
	} catch (const std::exception& ex) {
		std::fprintf(stderr, "[worker] fatal exception: %s\n", ex.what());
	} catch (...) {
		std::fprintf(stderr, "[worker] fatal unknown exception\n");
	}
	std::fflush(nullptr);
	std::_Exit(code);
}

[[nodiscard]] std::string describe(int status) {
	if (WIFEXITED(status)) {
		return "exited with status " + std::to_string(WEXITSTATUS(status));
	}
	if (WIFSIGNALED(status)) {
		const int sig{WTERMSIG(status)};
		const char* name{::strsignal(sig)};
		return "killed by signal " + std::to_string(sig) + " (" + (name != nullptr ? name : "?") + ")";
	}
	return "terminated abnormally";
}

}  // namespace

int run(const std::function<int()>& worker, const Options& options) {
	struct sigaction old_term{};
	struct sigaction old_int{};
	struct sigaction old_hup{};
	g_terminate_signal = 0;
	install_handlers(old_term, old_int, old_hup);

	auto backoff{options.min_backoff};
	int exit_code{EXIT_SUCCESS};

	const pid_t supervisor_pid{::getpid()};
	for (;;) {
		std::fflush(nullptr);  // do not duplicate buffered output into the child
		const auto started{std::chrono::steady_clock::now()};
		const pid_t pid{::fork()};

		if (pid < 0) {
			log_line(std::string{"fork failed: "} + std::strerror(errno));
			if (interruptible_sleep(backoff)) {
				break;
			}
			backoff = std::min(backoff * 2, options.max_backoff);
			continue;
		}

		if (pid == 0) {
			run_child(worker, supervisor_pid);
		}

		g_child_pid = pid;
		if (g_terminate_signal != 0) {
			(void)::kill(pid, g_terminate_signal);	// signal arrived between fork and here
		}

		int status{0};
		pid_t waited{-1};
		do {
			waited = ::waitpid(pid, &status, 0);
		} while (waited < 0 && errno == EINTR);
		g_child_pid = 0;

		if (waited < 0) {
			log_line(std::string{"waitpid failed: "} + std::strerror(errno));
			exit_code = EXIT_FAILURE;
			break;
		}

		if (g_terminate_signal != 0) {
			log_line("shutting down (worker " + describe(status) + ")");
			exit_code = EXIT_SUCCESS;
			break;
		}

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			exit_code = EXIT_SUCCESS;  // clean exit requested by the worker
			break;
		}

		const auto uptime{std::chrono::steady_clock::now() - started};
		if (uptime >= options.stable_after) {
			backoff = options.min_backoff;
		}
		log_line("worker " + describe(status) + " after " +
				 std::to_string(std::chrono::duration_cast<std::chrono::seconds>(uptime).count()) +
				 "s, restarting in " + std::to_string(backoff.count()) + "ms");
		if (interruptible_sleep(backoff)) {
			break;
		}
		backoff = std::min(backoff * 2, options.max_backoff);
	}

	restore_handlers(old_term, old_int, old_hup);
	return exit_code;
}

}  // namespace supervisor
