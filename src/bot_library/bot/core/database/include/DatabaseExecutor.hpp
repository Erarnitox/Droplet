/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include "IDatabaseConnectionProvider.hpp"
#include "RowDTOAdapter.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

/**
 * Executes queries/transactions against a connection provided by IDatabaseConnectionProvider.
 */
class DatabaseExecutor {
  public:
	explicit DatabaseExecutor(IDatabaseConnectionProvider& provider) noexcept : provider_(provider) {}

	DatabaseExecutor(const DatabaseExecutor&) = delete;
	DatabaseExecutor& operator=(const DatabaseExecutor&) = delete;

	[[nodiscard]] bool hasConnection() const noexcept { return provider_.hasConnection(); }

	template <typename... Types>
	[[nodiscard]] bool execQuery(const std::string& query, Types&&... args) noexcept;

	template <typename... Types>
	[[nodiscard]] RowDTOAdapter execSelect(const std::string& query, Types&&... args) noexcept;

	template <typename... Types>
	[[nodiscard]] std::vector<RowDTOAdapter> execSelectAll(const std::string& query, Types&&... args) noexcept;

	[[nodiscard]] static DatabaseExecutor& application_instance();

  private:
	IDatabaseConnectionProvider& provider_;
};

template <typename... Types>
[[nodiscard]] bool DatabaseExecutor::execQuery(const std::string& query, Types&&... args) noexcept {
	static thread_local int times = 0;
	try {
		NativeDatabase::Transaction txn(*provider_.getConnection());
		const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
		(void)txn.exec(query, params);
		txn.commit();
		times = 0;
		return true;
	} catch (const NativeDatabase::BrokenConnectionException&) {
		++times;
		if (times > 10) {
			times = 0;
			return false;
		}
		provider_.reconnect();
		return execQuery(query, std::forward<Types>(args)...);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\nInvalid exec for query:\n" << query << std::endl;
		return false;
	}
}

template <typename... Types>
[[nodiscard]] RowDTOAdapter DatabaseExecutor::execSelect(const std::string& query, Types&&... args) noexcept {
	static thread_local int times = 0;
	try {
		NativeDatabase::Transaction txn(*provider_.getConnection());
		const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
		const NativeDatabase::Result result = txn.exec(query, params);
		txn.commit();
		times = 0;
		return {result[0]};
	} catch (const NativeDatabase::BrokenConnectionException&) {
		++times;
		if (times > 10) {
			times = 0;
			return {NativeDatabase::Row()};
		}
		provider_.reconnect();
		return execSelect(query, std::forward<Types>(args)...);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\nInvalid selection for query:\n" << query << std::endl;
		return {{}};
	}
}

template <typename... Types>
[[nodiscard]] std::vector<RowDTOAdapter> DatabaseExecutor::execSelectAll(const std::string& query,
																		   Types&&... args) noexcept {
	static thread_local int times = 0;
	try {
		NativeDatabase::Transaction txn(*provider_.getConnection());
		const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
		const NativeDatabase::Result result = txn.exec(query, params);
		txn.commit();
		times = 0;
		std::vector<RowDTOAdapter> selection;
		for (int i{0}; i < result.size(); ++i) {
			selection.emplace_back(result[i]);
		}
		return selection;
	} catch (const NativeDatabase::BrokenConnectionException&) {
		++times;
		if (times > 10) {
			times = 0;
			return {};
		}
		provider_.reconnect();
		return execSelectAll(query, std::forward<Types>(args)...);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\nInvalid selection for query:\n" << query << std::endl;
		return {};
	}
}
