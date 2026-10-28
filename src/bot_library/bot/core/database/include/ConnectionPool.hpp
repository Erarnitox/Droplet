#pragma once

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "DatabaseBackend.hpp"

class ConnectionPool;

/**
 * Adds connection settings that make a hung or half-open database connection fail
 * in bounded time instead of blocking a worker thread forever: a connect timeout,
 * TCP keepalives and TCP user timeout. Settings already present in the string
 * (libpq "key=value" or postgres:// URI form) are left untouched.
 */
[[nodiscard]] std::string with_connection_defaults(std::string_view connection_string);

class ConnectionLease {
  public:
	ConnectionLease() noexcept = default;
	ConnectionLease(ConnectionPool& pool, std::unique_ptr<NativeDatabase::Connection> conn) noexcept;
	ConnectionLease(ConnectionLease&& other) noexcept;
	ConnectionLease& operator=(ConnectionLease&& other) noexcept;
	~ConnectionLease();

	ConnectionLease(const ConnectionLease&) = delete;
	ConnectionLease& operator=(const ConnectionLease&) = delete;

	[[nodiscard]] explicit operator bool() const noexcept {
		return static_cast<bool>(conn_);
	}

	void discard() noexcept {
		recycle_ = false;
	}

	[[nodiscard]] NativeDatabase::Connection& get() noexcept {
		return *conn_;
	}

  private:
	ConnectionPool* pool_{nullptr};
	std::unique_ptr<NativeDatabase::Connection> conn_;
	bool recycle_{true};
};

class ConnectionPool {
  public:
	ConnectionPool() = default;
	~ConnectionPool();

	ConnectionPool(const ConnectionPool&) = delete;
	ConnectionPool& operator=(const ConnectionPool&) = delete;

	[[nodiscard]] bool connect(const std::string& connection_string, std::size_t pool_size = 4);
	void disconnect() noexcept;
	[[nodiscard]] bool has_connection() const noexcept;
	[[nodiscard]] ConnectionLease acquire();

  private:
	friend class ConnectionLease;
	void release(std::unique_ptr<NativeDatabase::Connection> conn) noexcept;
	void discard_in_use() noexcept;
	[[nodiscard]] std::unique_ptr<NativeDatabase::Connection> open_connection() noexcept;

	std::string connection_string_;
	std::size_t max_size_{4};
	mutable std::mutex mutex_;
	std::condition_variable cv_;
	std::vector<std::unique_ptr<NativeDatabase::Connection>> idle_;
	std::size_t in_use_{0};
	std::size_t opening_{0};
	bool connected_{false};
	// after a failed attempt to open a connection, do not hammer the database
	// (and block every caller for the connect timeout) until this point in time
	std::chrono::steady_clock::time_point retry_not_before_{};
};
