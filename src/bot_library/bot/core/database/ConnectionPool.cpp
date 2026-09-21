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
				pool_->drop_and_replace();
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
			pool_->drop_and_replace();
		}
	}
}

ConnectionPool::~ConnectionPool() {
	disconnect();
}

bool ConnectionPool::connect(const std::string& connection_string, std::size_t pool_size) {
	disconnect();
	connection_string_ = connection_string;
	max_size_ = pool_size == 0 ? 1 : pool_size;

	try {
		std::lock_guard<std::mutex> lock(mutex_);
		for (std::size_t i = 0; i < max_size_; ++i) {
			auto conn = std::make_unique<NativeDatabase::Connection>(connection_string_);
			if (not conn->is_open()) {
				idle_.clear();
				connected_ = false;
				return false;
			}
			idle_.push_back(std::move(conn));
		}
		connected_ = true;
		return true;
	} catch (...) {
		idle_.clear();
		connected_ = false;
		return false;
	}
}

void ConnectionPool::disconnect() noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	idle_.clear();
	in_use_ = 0;
	connected_ = false;
}

bool ConnectionPool::has_connection() const noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	return connected_ && (not idle_.empty() || in_use_ > 0);
}

ConnectionLease ConnectionPool::acquire() {
	std::unique_lock<std::mutex> lock(mutex_);
	if (!cv_.wait_for(lock, std::chrono::seconds(5), [this] { return not connected_ || not idle_.empty(); })) {
		return {};
	}
	if (not connected_ || idle_.empty()) {
		return {};
	}
	auto conn = std::move(idle_.back());
	idle_.pop_back();
	++in_use_;
	lock.unlock();
	return ConnectionLease(*this, std::move(conn));
}

void ConnectionPool::release(std::unique_ptr<NativeDatabase::Connection> conn) noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	if (in_use_ > 0) {
		--in_use_;
	}
	if (connected_ && conn) {
		idle_.push_back(std::move(conn));
	}
	cv_.notify_one();
}

void ConnectionPool::drop_and_replace() noexcept {
	std::lock_guard<std::mutex> lock(mutex_);
	if (in_use_ > 0) {
		--in_use_;
	}
	if (connected_) {
		try {
			auto conn = std::make_unique<NativeDatabase::Connection>(connection_string_);
			if (conn->is_open()) {
				idle_.push_back(std::move(conn));
			}
		} catch (...) {
		}
	}
	cv_.notify_one();
}
