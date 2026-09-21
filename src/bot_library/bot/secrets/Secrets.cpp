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
#include <stdexcept>

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

std::string Secrets::requireSecret(const std::string& key, std::size_t min_len) const {
	const auto value{getSecret(key)};
	if (value.size() < min_len) {
		throw std::runtime_error("Secret '" + key + "' is missing or shorter than required");
	}
	return value;
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