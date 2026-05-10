/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "DatabaseExecutor.hpp"

#include "Database.hpp"

namespace {

class GlobalDatabaseConnectionProvider final : public IDatabaseConnectionProvider {
  public:
	[[nodiscard]] bool hasConnection() noexcept override {
		return Database::hasConnection();
	}

	void reconnect() noexcept override {
		Database::reconnect();
	}

	[[nodiscard]] NativeDatabase::Connection* getConnection() noexcept override {
		return Database::getConnection();
	}
};

}  // namespace

DatabaseExecutor& DatabaseExecutor::application_instance() {
	static GlobalDatabaseConnectionProvider provider{};
	static DatabaseExecutor executor{provider};
	return executor;
}
