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

namespace NativeDatabase {
    using Row = pqxx::row;
    using Connection = pqxx::connection;
    using BrokenConnectionException = pqxx::broken_connection;
    using Transaction = pqxx::work;
    using ParameterList = pqxx::params;
    using Result = pqxx::result;
}