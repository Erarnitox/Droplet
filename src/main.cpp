/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: Entry Point for the main bot executable
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string_view>

#include "BotLibrary.hpp"
#include "Supervisor.hpp"

namespace {

[[noreturn]] void on_terminate() {
	// Last-resort diagnostics for an exception nobody caught (e.g. a non-std
	// exception thrown on a library thread), then abort so a core dump is kept
	// and the supervisor / service manager restarts the bot.
	std::fputs("[fatal] std::terminate called", stderr);
	if (const auto current{std::current_exception()}) {
		try {
			std::rethrow_exception(current);
		} catch (const std::exception& ex) {
			std::fprintf(stderr, " with uncaught exception: %s", ex.what());
		} catch (...) {
			std::fputs(" with uncaught non-standard exception", stderr);
		}
	}
	std::fputs("\n", stderr);
	std::fflush(nullptr);
	std::abort();
}

int run_worker() {
	std::set_terminate(on_terminate);
	try {
		return run_bot();
	} catch (const std::exception& ex) {
		std::fprintf(stderr, "[fatal] unhandled exception in bot: %s\n", ex.what());
	} catch (...) {
		std::fputs("[fatal] unhandled non-standard exception in bot\n", stderr);
	}
	return EXIT_FAILURE;
}

}  // namespace

/**
 * @brief Entry Point of our bot.
 *
 * By default the bot runs in a supervised child process that is restarted with
 * exponential backoff if it ever dies. Set DROPLET_NO_SUPERVISOR=1 to run it
 * directly in this process (debuggers, valgrind, ...).
 *
 * @return returns error code to the OS
 */
int main() {
	// a write to a closed socket must produce an error code, not kill the process
	(void)std::signal(SIGPIPE, SIG_IGN);

	// logs must reach journald/stdout line by line, also right before a crash
	(void)std::setvbuf(stdout, nullptr, _IOLBF, 0);
	(void)std::setvbuf(stderr, nullptr, _IONBF, 0);
	std::ios::sync_with_stdio(true);

	const char* const no_supervisor{std::getenv("DROPLET_NO_SUPERVISOR")};
	if (no_supervisor != nullptr && std::string_view{no_supervisor} == "1") {
		return run_worker();
	}
	return supervisor::run(run_worker);
}
