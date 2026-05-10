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

#include <string>
#include <utility>
#include <vector>

#include "DatabaseBackend.hpp"
#include "DatabaseExecutor.hpp"
#include "RowDTOAdapter.hpp"

/**
 * This is a Database class to manage the connection to a Database
 * it is a wrapper around a Platform specific Database connection
 * for now this is a Postgres Database using the pqxx Library
 */
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
 * Legacy free functions delegate to DatabaseExecutor::application_instance() so call sites can migrate
 * gradually to injecting DatabaseExecutor.
 */
template <typename... Types>
[[nodiscard]] bool execQuery(const std::string& query, Types&&... args) noexcept {
	return DatabaseExecutor::application_instance().execQuery(query, std::forward<Types>(args)...);
}

template <typename... Types>
[[nodiscard]] RowDTOAdapter execSelect(const std::string& query, Types&&... args) noexcept {
	return DatabaseExecutor::application_instance().execSelect(query, std::forward<Types>(args)...);
}

template <typename... Types>
[[nodiscard]] std::vector<RowDTOAdapter> execSelectAll(const std::string& query, Types&&... args) noexcept {
	return DatabaseExecutor::application_instance().execSelectAll(query, std::forward<Types>(args)...);
}
}  // namespace database