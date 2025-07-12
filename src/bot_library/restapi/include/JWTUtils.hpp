/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#pragma once

#include <Poco/JSON/Object.h>

#include <string>

//-----------------------------------------------------
//
//-----------------------------------------------------
namespace JWTUtils {
std::string generateToken(const Poco::JSON::Object& claims, const std::string& secret, const std::string& issuer);
}