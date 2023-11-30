/*
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This Library combines all the
 * 	fuctionality of the bot into a static library
 * 	that can be used by the main binary and also
 * 	the unit_test binary.
 *
 *  Documentation: https://droplet.dropsoft.org/doxygen/bot_library
 */

#include "BotLibrary.hpp"

#include <Bot.hpp>
#include <Commands.hpp>
#include <Database.hpp>
#include <fstream>

/**
 * @brief this is the entry point of the binary that will start the bot
 * @return doesn't return anything
 */
void start_bot(const bool IS_TEST) {
	// initialize bot
	try {
		Bot::init(read_bot_token("bot_token.txt"));
	} catch (const char* const error_message) {
		std::cerr << error_message;
		return;
	}

	//-----------------------------------------------------------------------------
	// connect to the Database
	//-----------------------------------------------------------------------------
	try {
		const auto db_connection_string{read_database_credentials("db_connection.txt")};

		const auto connected{Database::connect(db_connection_string)};
		if (!connected) {
			return;
		}
	} catch (const char* const error_message) {
		std::cerr << error_message;
		return;
	}
	//------------------------------------------------------------------------------

	// Command Registration:
	Commands::registerCommands();

	if (IS_TEST) {
		return;
	}

	Bot::run();

	return;
}

/**
 * @brief reads the bot token from a file
 *
 * @param file the name of the file holding the bot token
 * @return returns the bot token as a std::string
 */
std::string read_bot_token(const std::string& file) {
	std::ifstream file_stream(file);
	std::string bot_token;

	if (file_stream.is_open()) {
		file_stream >> bot_token;
	} else {
		// something went wrong opening the file
		throw("ERROR: bot token file could not be opened!");
	}

	if (bot_token.size() < 1) {
		// something went wrong reading the token
		throw("ERROR: NO bot token found!");
	}

	return bot_token;
}

/**
 * @brief reads the connection string for the postgres database from a file
 *
 * @param file the name of the file holding the connection string
 * @return returns the connection string as a std::string
 */
std::string read_database_credentials(const std::string& file) {
	std::ifstream file_stream(file);
	std::string connection_string;

	if (file_stream.is_open()) {
		std::getline(file_stream, connection_string);
	} else {
		throw("ERROR: Cant read Database connection string!");
	}

	if (connection_string.size() < 1) {
		throw("ERROR: NO DATABASE CREDENTIALS PROVIDED!");
	}

	return connection_string;
}