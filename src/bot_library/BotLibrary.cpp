/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "BotLibrary.hpp"

#include <fstream>
#include <stdexcept>

#include "Application.hpp"

void start_bot(const bool IS_TEST) {
	Application app;
	app.run(IS_TEST);
}

std::string read_bot_token(const std::string& file) {
	std::ifstream file_stream(file);
	std::string bot_token;

	if (file_stream.is_open()) {
		file_stream >> bot_token;
	} else {
		throw std::runtime_error("ERROR: bot token file could not be opened!");
	}

	if (bot_token.size() < 1) {
		throw std::runtime_error("ERROR: NO bot token found!");
	}

	return bot_token;
}

std::string read_database_credentials(const std::string& file) {
	std::ifstream file_stream(file);
	std::string connection_string;

	if (file_stream.is_open()) {
		std::getline(file_stream, connection_string);
	} else {
		throw std::runtime_error("ERROR: Cant read Database connection string!");
	}

	if (connection_string.size() < 1) {
		throw std::runtime_error("ERROR: NO DATABASE CREDENTIALS PROVIDED!");
	}

	return connection_string;
}
