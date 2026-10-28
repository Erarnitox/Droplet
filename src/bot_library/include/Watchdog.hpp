/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: Detects a stalled event loop. The owner calls pet() regularly
 *  (from a Discord timer, which runs on the library's event-loop thread). If it
 *  is not called for `timeout`, the process exits so that the supervisor / service
 *  manager can restart it instead of leaving the bot silently hung.
 */

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>

class Watchdog {
  public:
	using ExpiredHandler = std::function<void(std::chrono::seconds stalled_for)>;

	/// @param on_expired called from the watchdog thread when the timeout elapsed;
	///        the default handler logs and terminates the process with exit code 70.
	explicit Watchdog(std::chrono::seconds timeout, ExpiredHandler on_expired = {});
	~Watchdog();

	Watchdog(const Watchdog&) = delete;
	Watchdog& operator=(const Watchdog&) = delete;

	void start();
	void stop() noexcept;
	void pet() noexcept;

  private:
	void loop();
	[[nodiscard]] static std::int64_t now_ms() noexcept;

	std::chrono::seconds timeout_;
	ExpiredHandler on_expired_;
	std::atomic<std::int64_t> last_pet_ms_;
	std::mutex mutex_;
	std::condition_variable cv_;
	bool stopping_{false};
	std::thread thread_;
};
