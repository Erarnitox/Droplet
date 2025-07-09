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

#include <iostream>
#include <string>
#include <vector>

#include "DatabaseBackend.hpp"
#include "RowDTOAdapter.hpp"

class Database {
  public:
	Database() = delete;

	[[nodiscard]] static bool connect(const std::string& db_name,
									  const std::string& user,
									  const std::string& password,
									  const std::string& host,
									  const std::string& port);

	[[nodiscard]] static bool connect(const std::string& connection_string);

	static bool hasConnection() noexcept;
	static void reconnect() noexcept;
	static void disconnect() noexcept;
	static NativeDatabase::Connection* getConnection() noexcept;
};

namespace database {

/**
 * @brief executes a query on the database
 *
 * @param query SQL query to be executed
 * @param args the arguments the query takes
 * @return error code if the query was executed
 */
template <typename... Types>
[[nodiscard("You need to check if the Query was executed on the Database!")]] bool execQuery(const std::string& query,
																							 Types&&... args) noexcept {
	static int times = 0;
	try {
		NativeDatabase::Transaction txn(*Database::getConnection());

		// perform the database transaction
		const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
		const NativeDatabase::Result result = txn.exec(query, params);

		txn.commit();

		times = 0;
		return true;
	} catch (const NativeDatabase::BrokenConnectionException& e) {
		++times;
		if (times > 10) {
			times = 0;
			return false;
		}
		Database::reconnect();
		return execQuery(query, args...);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\nInvalid exec for query:\n" << query << std::endl;
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
[[nodiscard]] RowDTOAdapter execSelect(const std::string& query, Types&&... args) noexcept {
	static int times = 0;
	try {
		NativeDatabase::Transaction txn(*Database::getConnection());

		// perform the database transaction
		const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
		const NativeDatabase::Result result = txn.exec(query, params);
		txn.commit();

		times = 0;

		return {result[0]};
	} catch (const NativeDatabase::BrokenConnectionException& e) {
		++times;
		if (times > 10) {
			times = 0;
			return {NativeDatabase::Row()};
		}
		Database::reconnect();
		return execSelect(query, args...);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\nInvalid selection for query:\n" << query << std::endl;
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
[[nodiscard]] std::vector<RowDTOAdapter> execSelectAll(const std::string& query, Types&&... args) noexcept {
	static int times = 0;
	try {
		NativeDatabase::Transaction txn(*Database::getConnection());

		// perform the database transaction
		const NativeDatabase::ParameterList params{std::forward<Types>(args)...};
		const NativeDatabase::Result result = txn.exec(query, params);

		txn.commit();

		times = 0;

		std::vector<RowDTOAdapter> selection;
		for (int i{0}; i < result.size(); ++i) {
			selection.emplace_back(result[i]);
		}

		return selection;
	} catch (const NativeDatabase::BrokenConnectionException& e) {
		++times;
		if (times > 10) {
			times = 0;
			return {NativeDatabase::Row()};
		}
		Database::reconnect();
		return execSelectAll(query, args...);
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\nInvalid selection for query:\n" << query << std::endl;
		return {{}};  // return an empty selection
	}
}
}  // namespace database