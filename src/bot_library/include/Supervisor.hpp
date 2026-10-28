/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: Minimal in-binary process supervisor. Runs the real bot in a
 *  child process and restarts it (with exponential backoff) whenever it dies
 *  abnormally: uncaught exception, abort, segfault, OOM kill, watchdog exit.
 *  This keeps the bot alive even if the service manager has no Restart= policy.
 */

#pragma once

#include <chrono>
#include <functional>

namespace supervisor {

struct Options {
	std::chrono::milliseconds min_backoff{std::chrono::seconds{2}};
	std::chrono::milliseconds max_backoff{std::chrono::seconds{60}};
	// a worker that stayed up at least this long resets the backoff to min_backoff
	std::chrono::seconds stable_after{std::chrono::minutes{5}};
};

/**
 * @brief Run `worker` in a forked child and restart it until it exits with status 0
 * or this process receives SIGTERM/SIGINT/SIGHUP (which are forwarded to the child).
 *
 * Must be called before any thread has been started in the calling process.
 *
 * @return the exit code the supervising process should exit with
 */
[[nodiscard]] int run(const std::function<int()>& worker, const Options& options = {});

}  // namespace supervisor
