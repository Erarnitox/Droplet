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

#include "Database.hpp"

#include <format>
#include <memory>

#include "DatabaseBackend.hpp"

static std::unique_ptr<NativeDatabase::Connection> conn;

/**
 * @brief connect to a postgres database
 *
 * @param db_name name of the database
 * @param user database user that will be used for the log in
 * @param password the password that will be used to log in
 * @param host the ip or domain of the database server
 * @param port the port the database service is listening on
 * @return returns a bool if the connection was successful
 */
bool Database::connect(const std::string& db_name,
					   const std::string& user,
					   const std::string& password,
					   const std::string& host,
					   const std::string& port) {
	Database::disconnect();
	conn = std::make_unique<NativeDatabase::Connection>(
		std::format("dbname={} user={} password={} hostaddr={} port={}", db_name, user, password, host, port));

	return conn->is_open();
}

/**
 * @brief connect to a postgres database
 *
 * @param connection_string connection string used to connect to the database
 * @return returns a bool if the connection was successful
 */
bool Database::connect(const std::string& connection_string) {
	conn = std::make_unique<NativeDatabase::Connection>(connection_string);
	return conn->is_open();
}

/**
 * @brief disconnect from a postgres database
 * @param connection_string connection string used to connect to the database
 * @return returns a bool if the connection was successful
 */
void Database::disconnect() noexcept {
	conn = nullptr;
}

/**
 * @brief checks if the database has an active connection
 * @return returns true if the database is connected, false otherwise
 */
bool Database::hasConnection() noexcept {
	if (not conn)
		return false;
	return conn->is_open();
}

/**
 * @brief reconnects to the database server if the connection was lost
 * @return doesn't return anything
 */
void Database::reconnect() noexcept {
	static int times = 0;
	if (not conn)
		return;

	try {
		++times;
		bool connected{conn && conn->is_open()};
		if (not connected) {
			// conn->activate();
			connected = Database::connect(conn->connection_string());
		}
		if (connected)
			times = 0;
	} catch (const NativeDatabase::BrokenConnectionException& e) {
		if (times > 10) {
			times = 0;
			return;
		}
		Database::reconnect();
	} catch (...) {
		std::puts("Error: reconnecting failed...\n");
	}
}

/**
 * @brief returns the database connection. Reconnects if the Database has no active connection
 * @return the active database connection as pqxx::connection pointer
 */
NativeDatabase::Connection* Database::getConnection() noexcept {
	if (not Database::hasConnection()) {
		Database::reconnect();
	}
	return conn.get();
}