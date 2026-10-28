/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "ConnectionPool.hpp"

#include <chrono>

ConnectionLease::ConnectionLease(ConnectionPool& pool, std::unique_ptr<NativeDatabase::Connection> conn) noexcept
	: pool_(&pool), conn_(std::move(conn)) {
}

ConnectionLease::ConnectionLease(ConnectionLease&& other) noexcept
	: pool_(other.pool_), conn_(std::move(other.conn_)), recycle_(other.recycle_) {
	other.pool_ = nullptr;
}

ConnectionLease& ConnectionLease::operator=(ConnectionLease&& other) noexcept {
	if (this != &other) {
		if (pool_ && conn_) {
			if (recycle_) {
				pool_->release(std::move(conn_));
			} else {
				conn_.reset();
				pool_->discard_in_use();
			}
		}
		pool_ = other.pool_;
		conn_ = std::move(other.conn_);
		recycle_ = other.recycle_;
		other.pool_ = nullptr;
	}
	return *this;
}

ConnectionLease::~ConnectionLease() {
	if (pool_ && conn_) {
		if (recycle_) {
			pool_->release(std::move(conn_));
		} else {
			conn_.reset();
			pool_->discard_in_use();
		}
	}
}

ConnectionPool::~ConnectionPool() {
	disconnect();
}

namespace {

constexpr auto k_acquire_timeout = std::chrono::seconds(5);
constexpr auto k_open_retry_delay = std::chrono::seconds(2);

[[nodiscard]] bool contains_key(std::string_view haystack, std::string_view key) {
	return haystack.find(key) != std::string_view::npos;
}

}  // namespace

std::string with_connection_defaults(std::string_view connection_string) {
	std::string result{connection_string};
	const bool is_uri = result.starts_with("postgres://") || result.starts_with("postgresql://");

	const auto add = [&](std::string_view key, std::string_view value) {
		if (contains_key(connection_string, std::string{key} + "=")) {
			return;
		}
		if (is_uri) {
			result += result.find('?') == std::string::npos ? '?' : '&';
			result += std::string{key} + "=" + std::string{value};
		} else {
			result += ' ';
			result += std::string{key} + "=" + std::string{value};
		}
	};

	add("connect_timeout", "10");
	add("keepalives", "1");
	add("keepalives_idle", "30");
	add("keepalives_interval", "10");
	add("keepalives_count", "3");
	add("tcp_user_timeout", "30000");
	return result;
}

std::unique_ptr<NativeDatabase::Connection> ConnectionPool::open_connection() noexcept {
	try {
		auto conn = std::make_unique<NativeDatabase::Connection>(connection_string_);
		if (conn->is_open()) {
			return conn;
		}
	} catch (...) {
	}
	return nullptr;
}

bool ConnectionPool::connect(const std::string& connection_string, std::size_t pool_size) {
	disconnect();
	{
		std::lock_guard<std::mutex> lock(mutex_);
		connection_string_ = with_connection_defaults(connection_string);
		max_size_ = pool_size == 0 ? 1 : pool_size;
		retry_not_before_ = {};
	}

	// the first connection proves that the credentials / host are usable;
	// the remaining ones are opened lazily by acquire()
	auto conn = open_connection();
	if (not conn) {
		return false;
	}

	std::lock_guard<std::mutex> lock(mutex_);
	idle_.push_back(std::move(conn));
	connected_ = true;
	return true;
}

void ConnectionPool::disconnect() noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	idle_.clear();
	in_use_ = 0;
	connected_ = false;
	cv_.notify_all();
}

bool ConnectionPool::has_connection() const noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	return connected_;
}

ConnectionLease ConnectionPool::acquire() {
	std::unique_lock<std::mutex> lock(mutex_);
	const auto deadline = std::chrono::steady_clock::now() + k_acquire_timeout;

	while (connected_) {
		// reuse an idle connection, dropping any that are already known to be closed
		while (not idle_.empty()) {
			auto conn = std::move(idle_.back());
			idle_.pop_back();
			if (conn && conn->is_open()) {
				++in_use_;
				lock.unlock();
				return ConnectionLease(*this, std::move(conn));
			}
		}

		// below the limit: open a new connection (this is also how the pool
		// recovers after the database was restarted or unreachable)
		if (in_use_ + opening_ < max_size_) {
			if (std::chrono::steady_clock::now() < retry_not_before_) {
				return {};	// the database was unreachable a moment ago: fail fast
			}
			++opening_;
			lock.unlock();
			auto conn = open_connection();	// bounded by connect_timeout
			lock.lock();
			--opening_;
			if (not connected_) {
				return {};
			}
			if (conn) {
				retry_not_before_ = {};
				++in_use_;
				lock.unlock();
				return ConnectionLease(*this, std::move(conn));
			}
			retry_not_before_ = std::chrono::steady_clock::now() + k_open_retry_delay;
			cv_.notify_one();
			return {};
		}

		// pool exhausted: wait for a connection to be returned
		if (cv_.wait_until(lock, deadline) == std::cv_status::timeout) {
			return {};
		}
	}
	return {};
}

void ConnectionPool::release(std::unique_ptr<NativeDatabase::Connection> conn) noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	if (in_use_ > 0) {
		--in_use_;
	}
	if (connected_ && conn) {
		try {
			idle_.push_back(std::move(conn));
		} catch (...) {
		}
	}
	cv_.notify_one();
}

void ConnectionPool::discard_in_use() noexcept {
	// the broken connection is simply dropped; acquire() opens a replacement on demand
	std::lock_guard<std::mutex> lock(mutex_);
	if (in_use_ > 0) {
		--in_use_;
	}
	cv_.notify_one();
}
