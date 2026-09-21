#pragma once

#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "DatabaseBackend.hpp"

class ConnectionPool;

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
	void drop_and_replace() noexcept;

	std::string connection_string_;
	std::size_t max_size_{4};
	mutable std::mutex mutex_;
	std::condition_variable cv_;
	std::vector<std::unique_ptr<NativeDatabase::Connection>> idle_;
	std::size_t in_use_{0};
	bool connected_{false};
};
