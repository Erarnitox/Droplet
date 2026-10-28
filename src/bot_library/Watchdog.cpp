/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "Watchdog.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <utility>

namespace {
constexpr int k_exit_stalled{70};  // EX_SOFTWARE
}

Watchdog::Watchdog(std::chrono::seconds timeout, ExpiredHandler on_expired)
	: timeout_(timeout), on_expired_(std::move(on_expired)), last_pet_ms_(now_ms()) {
	if (not on_expired_) {
		on_expired_ = [](std::chrono::seconds stalled_for) {
			std::fprintf(stderr,
						 "[watchdog] event loop did not respond for %lld s - exiting so the service can restart\n",
						 static_cast<long long>(stalled_for.count()));
			std::fflush(nullptr);
			std::_Exit(k_exit_stalled);
		};
	}
}

Watchdog::~Watchdog() {
	stop();
}

std::int64_t Watchdog::now_ms() noexcept {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
		.count();
}

void Watchdog::pet() noexcept {
	last_pet_ms_.store(now_ms(), std::memory_order_relaxed);
}

void Watchdog::start() {
	std::lock_guard<std::mutex> lock(mutex_);
	if (thread_.joinable()) {
		return;
	}
	stopping_ = false;
	last_pet_ms_.store(now_ms(), std::memory_order_relaxed);
	thread_ = std::thread([this] { loop(); });
}

void Watchdog::stop() noexcept {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		stopping_ = true;
	}
	cv_.notify_all();
	if (thread_.joinable()) {
		thread_.join();
	}
}

void Watchdog::loop() {
	const auto poll{std::chrono::milliseconds{std::max<std::int64_t>(50, timeout_.count() * 1000 / 10)}};
	std::unique_lock<std::mutex> lock(mutex_);
	while (not stopping_) {
		if (cv_.wait_for(lock, poll, [this] { return stopping_; })) {
			return;
		}
		const auto stalled_ms{now_ms() - last_pet_ms_.load(std::memory_order_relaxed)};
		if (stalled_ms >= std::chrono::duration_cast<std::chrono::milliseconds>(timeout_).count()) {
			lock.unlock();
			on_expired_(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds{stalled_ms}));
			return;
		}
	}
}
