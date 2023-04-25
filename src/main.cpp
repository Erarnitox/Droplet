#include "main.hpp"
#include "database.hpp"
#include "resource_man.hpp"
#include "roles.hpp"
#include "user_man.hpp"
#include <dpp/dispatcher.h>
#include <dpp/presence.h>
#include <fmt/core.h>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////////
auto main() -> int {
	// initialize bot
	dpp::cluster bot(read_bot_token("bot_token.txt"));
	bot.on_log(dpp::utility::cout_logger());

	//-----------------------------------------------------------------------------
	// connect to the Database
	//-----------------------------------------------------------------------------
	const auto db_connection_string{ read_database_credentials("db_connection.txt") };
	auto db{ Database(db_connection_string) };
	if(!db.has_connection()) {
		// something went wrong connecting to the database
		return -1;
	}
	//------------------------------------------------------------------------------

	// list of slash commands
	std::vector<dpp::slashcommand> global_command_list;
	register_global_slash_commands(global_command_list, bot);

	// when a member joins the guild
	bot.on_guild_member_add([&bot, &db](const dpp::guild_member_add_t& event) -> void {
		welcome_member(event, bot);
	});

	// when a member leaves the guild
	bot.on_guild_member_remove([&bot, &db](const dpp::guild_member_remove_t& event) -> void {
		leave_member(event, bot);
	});

	// register slash commands
	bot.on_ready([&bot, &global_command_list, &db](const dpp::ready_t& event) -> void {
		try{
			if(dpp::run_once<struct register_bot_commands>()) {
				bot.global_bulk_command_create(global_command_list);
			}
		} catch(...) {
			// smeting went wrong registering commands
		}
	});

	// handle slash commands
	bot.on_slashcommand([&bot, &global_command_list, &db](const dpp::slashcommand_t& event) -> void {
		try{
			handle_global_slash_commands(event, bot, global_command_list, db);
		} catch(...){
			event.reply("I could not process that input! ...");
		}
	});

	// handle button clicks
	bot.on_button_click([&bot, &db](const dpp::button_click_t& event) -> void {
		try{
			handle_button_clicks(event, bot);
		} catch(...) {
			event.reply("Could not process the button clicked! ...");	
		}
	});

	// handle form submits
	bot.on_form_submit([&bot, &db](const dpp::form_submit_t & event) -> void {
		try{
			handle_form_submits(event, bot, db);
		} catch(...) {
			event.reply("I could not handle the provided input! ...");
		}
	});

	// handle added reactions
	bot.on_message_reaction_add([&bot, &db](const dpp::message_reaction_add_t & event) -> void {
		try{
			handle_reaction_added(event, bot, db);
		} catch(...) {
			// something went wrong handeling the reaction
		}
	});

	// handle remove reactions
	bot.on_message_reaction_remove([&bot, &db](const dpp::message_reaction_remove_t & event) -> void {
		try{
			handle_reaction_removed(event, bot, db);
		} catch(...) {
			// something went wrong handeling the removed reaction
		}
	});

	// start execution of the bot
	bot.start(dpp::st_wait);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// READ BOT TOKEN FROM FILE
//////////////////////////////////////////////////////////////////////////////
auto read_bot_token(const std::string& file) -> std::string { 
	std::ifstream file_stream(file);
	std::string bot_token;
	
	if(file_stream.is_open()) {
		file_stream >> bot_token;	
	} else {
		// something went wrong opening the file
		throw("ERROR: bot token file could not be opened!");
	}

	if(bot_token.size() < 1) {
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
	
	if(file_stream.is_open()) {
		std::getline(file_stream, connection_string);	
	} else {
		throw("ERROR: Cant read Database connection string!");
	}

	if(connection_string.size() < 1)
		throw("ERROR: NO DATABASE CREDENTIALS PROVIDED!");

	return connection_string;
}

//////////////////////////////////////////////////////////////////////////////
// RGISTER ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
	// user commands
	resource_man::register_global_slash_commands(command_list, bot);
	user_man::register_global_slash_commands(command_list, bot);

	// admin commands
	core::register_global_slash_commands(command_list, bot);
	roles::register_global_slash_commands(command_list, bot);
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
auto handle_global_slash_commands(
	const dpp::slashcommand_t& event, 
	dpp::cluster& bot, 
	const std::vector<dpp::slashcommand>& command_list,
	Database& db
) -> void {

	// user commands
	resource_man::handle_global_slash_commands(event);
	user_man::handle_global_slash_commands(event, bot);

	// admin commands
	core::handle_global_slash_commands(event, bot, command_list);
	roles::handle_global_slash_commands(event, bot, db);
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void {
	roles::handle_button_clicks(event, bot);
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE FORM SUBMITS
//////////////////////////////////////////////////////////////////////////////
auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot, Database& db) -> void {
	roles::handle_form_submits(event, bot, db);
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ADDED REACTIONS
//////////////////////////////////////////////////////////////////////////////
auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot, Database& db) -> void {
	roles::handle_reaction_added(event, bot, db);
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE REMOVED REACTIONS
//////////////////////////////////////////////////////////////////////////////
auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot, Database& db) -> void {
	roles::handle_reaction_removed(event, bot, db);
}

//////////////////////////////////////////////////////////////////////////////
// WELCOME NEW MEMBERS TO A GUILD
//////////////////////////////////////////////////////////////////////////////
auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void {
	user_man::welcome_member(event, bot);
}

//////////////////////////////////////////////////////////////////////////////
// REMOVE MEMBERS FROM A GUILD
//////////////////////////////////////////////////////////////////////////////
auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void {
	user_man::leave_member(event, bot);
}