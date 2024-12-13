/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class manages Database access
 *  and provides a high level API for internal use
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/database
 */

#pragma once

#include <cstddef>
// #include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <variant>
#include <vector>
// #include <format>

#include "RowDTOAdapter.hpp"

class Database {
  public:
	Database() = delete;

	[[nodiscard]] static auto connect(const std::string& db_name,
									  const std::string& user,
									  const std::string& password,
									  const std::string& host,
									  const std::string& port) -> bool;

	[[nodiscard]] static auto connect(const std::string& connection_string) -> bool;

	static auto hasConnection() noexcept -> bool;
	static auto reconnect() noexcept -> void;
	static auto disconnect() noexcept -> void;
	static auto getConnection() noexcept -> pqxx::connection*;
};

namespace database {

// Compile time "for" loop
template <size_t I = 0, typename... Types>
constexpr void assignResults(const pqxx::result& result, std::vector<std::variant<Types...>>& args) {
	if constexpr (I < sizeof...(Types)) {
		args[I] = result.at(0, I).template get<std::variant_alternative_t<I, std::variant<Types...>>>().value();
		assignResults<I + 1>(result, args);
	}
}

/**
 * @brief executes a query on the database
 *
 * @param query SQL query to be executed
 * @param args the arguments the query takes
 * @return error code if the query was executed
 */
template <typename... Types>
[[nodiscard("You need to check if the Query was executed on the Database!")]] auto execQuery(const std::string& query,
																							 Types&&... args) noexcept
	-> bool {
	static int times = 0;
	try {
		pqxx::work txn(*Database::getConnection());

		// perform the database transaction
		pqxx::result result = txn.exec_params(query, std::forward<Types>(args)...);
		txn.commit();

		times = 0;
		return true;
	} catch (const pqxx::broken_connection& e) {
		++times;
		if (times > 10) {
			times = 0;
			return false;
		}
		Database::reconnect();
		return execQuery(query, args...);
	} catch (...) {
		// std::cout << std::format("Invalid exec for query:{}\n", query);
		return false;
	}
}

/**
 * @brief executes a Select on the database
 *
 * @param query SQL select query to be executed
 * @param args the arguments the query takes
 * @return RowDTOAdapter that represents the selected database row
 */
template <typename... Types>
[[nodiscard]] auto execSelect(const std::string& query, Types&&... args) noexcept -> RowDTOAdapter {
	static int times = 0;
	try {
		pqxx::work txn(*Database::getConnection());

		// perform the database transaction
		pqxx::result result = txn.exec_params(query, std::forward<Types>(args)...);
		txn.commit();

		times = 0;

		return {result[0]};
	} catch (const pqxx::broken_connection& e) {
		++times;
		if (times > 10) {
			times = 0;
			return {pqxx::row()};
		}
		Database::reconnect();
		return execSelect(query, args...);
	} catch (...) {
		// std::cout << std::format("Invalid selection for query: {}\n", query);
		return {{}};  // return an empty selection
	}
}

/**
 * @brief executes a Select on the database
 *
 * @param query SQL select query to be executed
 * @param args the arguments the query takes
 * @return std::vector<RowDTOAdapter> that represents the selected database rows
 */
template <typename... Types>
[[nodiscard]] auto execSelectAll(const std::string& query, Types&&... args) noexcept -> std::vector<RowDTOAdapter> {
	static int times = 0;
	try {
		pqxx::work txn(*Database::getConnection());

		// perform the database transaction
		pqxx::result result = txn.exec_params(query, std::forward<Types>(args)...);
		txn.commit();

		times = 0;

		std::vector<RowDTOAdapter> selection;
		for (int i{0}; i < result.size(); ++i) {
			selection.emplace_back(result[i]);
		}

		return selection;
	} catch (const pqxx::broken_connection& e) {
		++times;
		if (times > 10) {
			times = 0;
			return {pqxx::row()};
		}
		Database::reconnect();
		return execSelectAll(query, args...);
	} catch (...) {
		// std::cout << std::format("Invalid selection for query: {}\n", query);
		return {{}};  // return an empty selection
	}
}
}  // namespace database