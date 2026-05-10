/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include "DatabaseBackend.hpp"

/**
 * Abstraction over database connectivity so executors and repositories do not
 * depend directly on the concrete Database singleton (DIP).
 */
class IDatabaseConnectionProvider {
  public:
	virtual ~IDatabaseConnectionProvider() = default;

	[[nodiscard]] virtual bool hasConnection() noexcept = 0;
	virtual void reconnect() noexcept = 0;
	[[nodiscard]] virtual NativeDatabase::Connection* getConnection() noexcept = 0;
};
