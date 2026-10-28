/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This file should abstract away the underlying database engine.
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/database
 */

#pragma once

// disable deprecation warning
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcpp"
#endif

#include <pqxx/pqxx>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

/**
 * For now these are just some type definitions to abstract away the pqxx native types.
 * Later this could be expanded to an interface to allow for multiple different database libraries
 */
namespace NativeDatabase {
using Row = pqxx::row;
// libpqxx >= 8: indexing/iterating a result yields a non-owning row_ref
using RowRef = pqxx::row_ref;
using Connection = pqxx::connection;
using BrokenConnectionException = pqxx::broken_connection;
// libpqxx >= 8 derives protocol_violation from sql_error instead of broken_connection;
// it still poisons the connection, so it is retried like a broken connection.
using ProtocolViolationException = pqxx::protocol_violation;
using Transaction = pqxx::work;
using ParameterList = pqxx::params;
using Result = pqxx::result;
}  // namespace NativeDatabase