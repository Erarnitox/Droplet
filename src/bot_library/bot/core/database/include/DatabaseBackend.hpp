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
using Connection = pqxx::connection;
using BrokenConnectionException = pqxx::broken_connection;
using Transaction = pqxx::work;
using ParameterList = pqxx::params;
using Result = pqxx::result;
}  // namespace NativeDatabase