/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class provides a Singleton to access secrets
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/database
 */

#pragma once

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include <string>

//-----------------------------------------------------
//
//-----------------------------------------------------
class Secrets {
  public:
	static Secrets& getInstance();
	[[nodiscard]] std::string getSecret(const std::string& key, const std::string& fallback = "") const;

  private:
	Secrets();
	Secrets(const Secrets&) = delete;
	Secrets& operator=(const Secrets&) = delete;

	Poco::JSON::Object::Ptr secrets;
};