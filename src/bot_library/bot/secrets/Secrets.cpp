/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class manages Secrets
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/database
 */

#include "Secrets.hpp"

#include <fstream>

//-----------------------------------------------------
//
//-----------------------------------------------------
Secrets& Secrets::getInstance() {
	static Secrets instance;
	return instance;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
std::string Secrets::getSecret(const std::string& key, const std::string& fallback) const {
	if (secrets->has(key)) {
		return secrets->getValue<std::string>(key);
	} else {
		return fallback;
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
Secrets::Secrets() {
	std::ifstream file("secrets.json");
	if (!file.is_open()) {
		throw std::runtime_error("Could not open secrets.json");
	}

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var result = parser.parse(file);
	secrets = result.extract<Poco::JSON::Object::Ptr>();
}