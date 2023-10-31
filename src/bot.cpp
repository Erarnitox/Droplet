#include "bot.hpp"

#include <dpp/dispatcher.h>
#include <dpp/misc-enum.h>
#include <dpp/presence.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>

#include <CommandRegister.hpp>

#include <Bot.hpp>
#include <Core.hpp>
#include <Database.hpp>
#include <resource_man.hpp>
#include <roles.hpp>
#include <string>
#include <user_man.hpp>

//////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////////
auto start_bot() -> void {
	// initialize bot
	Bot::set_toke(read_bot_token("bot_token.txt"));

	//-----------------------------------------------------------------------------
	// connect to the Database
	//-----------------------------------------------------------------------------
	const auto db_connection_string{read_database_credentials("db_connection.txt")};

	const auto connected{Database::connect(db_connection_string)};
	if (!connected) {
		return;
	}
	//------------------------------------------------------------------------------

	// Command Registration:
	CommandRegister::registerCommands();
	
	Bot::run();

	return;
}

//////////////////////////////////////////////////////////////////////////////
// READ BOT TOKEN FROM FILE
//////////////////////////////////////////////////////////////////////////////
auto read_bot_token(const std::string& file) -> std::string {
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

//////////////////////////////////////////////////////////////////////////////
// READ DATABASE CREDENTIALS FROM FILE
//////////////////////////////////////////////////////////////////////////////
auto read_database_credentials(const std::string& file) -> std::string {
	std::ifstream file_stream(file);
	std::string connection_string;

	if (file_stream.is_open()) {
		std::getline(file_stream, connection_string);
	} else {
		throw("ERROR: Cant read Database connection string!");
	}

	if (connection_string.size() < 1)
		throw("ERROR: NO DATABASE CREDENTIALS PROVIDED!");

	return connection_string;
}