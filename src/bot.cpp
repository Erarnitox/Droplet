#include "bot.hpp"

#include <Database.hpp>
#include <Core.cpp>
#include <resource_man.hpp>
#include <user_man.hpp>
#include <Commands.hpp>
#include <ChallengeRoleCommand.hpp>
#include <CommandRegister.hpp>
#include <Commands.hpp>
#include <roles.hpp>
#include <user_man.hpp>

#include <dpp/dispatcher.h>
#include <dpp/misc-enum.h>
#include <dpp/presence.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/os.h>

#include <string>

//////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////////
auto start_bot() -> void {
	// initialize bot
	dpp::cluster bot(read_bot_token("bot_token.txt"));

	//-----------------------------------------------------------------------------
	// Functionality for Logging
	//-----------------------------------------------------------------------------
	auto err_log_out{ fmt::output_file("error.log", fmt::file::WRONLY | fmt::file::CREATE | fmt::file::APPEND | fmt::file::TRUNC) };
	auto log_out{ fmt::output_file("droplet.log", fmt::file::WRONLY | fmt::file::CREATE | fmt::file::APPEND | fmt::file::TRUNC) };

	bot.on_log([&log_out, &err_log_out](const dpp::log_t& event) {
		if (event.severity == dpp::ll_trace) {
			fmt::print(
				fg(fmt::color::green), 
				"[TRACE]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message
			);
		} else if (event.severity == dpp::ll_debug) {
			fmt::print(
				fg(fmt::color::blue), 
				"[DEBUG]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message
			);
		} else if (event.severity == dpp::ll_info) {
			fmt::print(
				fg(fmt::color::antique_white), 
				"[INFO]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message
			);
			log_out.print("[INFO]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_warning) {
			fmt::print(
				fg(fmt::color::yellow), 
				"[WARNING]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message
			);
			log_out.print("[WARNING]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_error) {
			fmt::print(
				fg(fmt::color::orange_red), 
				"[ERROR]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message
			);
			log_out.print("[ERROR]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
			err_log_out.print("[ERROR]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_critical) {
			fmt::print(
				fg(fmt::color::red), 
				"[CRITICAL]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message
			);
			log_out.print("[CRITICAL]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
			err_log_out.print("[CRITICAL]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		}

		if(event.severity > dpp::ll_debug) {
			log_out.flush();
			err_log_out.flush();
		}

	});

	bot.log(dpp::loglevel::ll_trace, "Bot started!");
	
	//-----------------------------------------------------------------------------
	// connect to the Database
	//-----------------------------------------------------------------------------
	const auto db_connection_string{ read_database_credentials("db_connection.txt") };
	
	const auto connected{ Database::connect(db_connection_string) };
	if(!connected) {
		bot.log(dpp::loglevel::ll_error, "Connection to Database was not successful!");
		return;
	}
	//------------------------------------------------------------------------------
	std::vector<dpp::slashcommand> global_command_list;

	// Command Registration:
	CommandRegister::registerCommands();

	// list of slash commands
	register_global_slash_commands(global_command_list, bot);

	// register slash commands
	bot.on_ready([&bot, &global_command_list](const dpp::ready_t& event) -> void {
		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "https://droplet.dropsoft.org"));
	
		try{
			if(dpp::run_once<struct register_bot_commands>()) {
				bot.log(dpp::ll_trace, "Registering Slash commands...");
				bot.global_bulk_command_create(global_command_list);

				// delete commands that are not implemented yet
        		//bot.global_command_delete(1090370434232483985);
        		//bot.global_command_delete(1088242804418949181);
			}
		} catch(...) {
			bot.log(dpp::loglevel::ll_error, "Something went wrong registering the slash commands!");
		}
	});

	// handle slash commands
	bot.on_slashcommand([&bot, &global_command_list](const dpp::slashcommand_t& event) -> void {
		try{
			handle_global_slash_commands(event, bot, global_command_list);
		} catch(...){
			bot.log(dpp::loglevel::ll_warning, fmt::format("Unhandeled exception occured in 'handle_global_slash_commands' input was {}", event.command.msg.content ));
			event.reply("I could not process that input! ...");
		}
	});

	// when a member joins the guild
	bot.on_guild_member_add([&bot](const dpp::guild_member_add_t& event) -> void {
		bot.log(dpp::ll_info, fmt::format("A new member: {} joined the guild: {}", event.added.get_nickname(), event.adding_guild->name));
		welcome_member(event, bot);
	});

	// when a member leaves the guild
	bot.on_guild_member_remove([&bot](const dpp::guild_member_remove_t& event) -> void {
		bot.log(dpp::ll_info, fmt::format("{} left the guild {}", event.removed.username, event.removing_guild->name));
		leave_member(event, bot);
	});

	// handle button clicks
	bot.on_button_click([&bot](const dpp::button_click_t& event) -> void {
		try{
			handle_button_clicks(event, bot);
		} catch(...) {
			bot.log(dpp::loglevel::ll_warning, "Unhandeled exception occured in 'handle_button_clicks'" );
			event.reply("Could not process the button clicked! ...");	
		}
	});

	// handle form submits
	bot.on_form_submit([&bot](const dpp::form_submit_t & event) -> void {
		try{
			handle_form_submits(event, bot);
			bot.log(dpp::loglevel::ll_info, fmt::format("[{}] form submitted with input: {}", event.custom_id, std::get<std::string>(event.components.at(0).components.at(0).value)));
		} catch(...) {
			bot.log(dpp::loglevel::ll_warning, fmt::format("Unhandeled exception occured in 'handle_form_submits', input was {}", std::get<std::string>(event.components.at(0).components.at(0).value)));
			event.reply("I could not handle the provided input! ...");
		}
	});

	// handle added reactions
	bot.on_message_reaction_add([&bot](const dpp::message_reaction_add_t & event) -> void {
		try{
			handle_reaction_added(event, bot);
		} catch(...) {
			bot.log(dpp::loglevel::ll_warning, "Unhandeled exception occured in 'handle_reaction_added'" );
		}
	});

	// handle remove reactions
	bot.on_message_reaction_remove([&bot](const dpp::message_reaction_remove_t & event) -> void {
		try{
			handle_reaction_removed(event, bot);
		} catch(...) {
			bot.log(dpp::loglevel::ll_warning, "Unhandeled exception occured in 'handle_reaction_removed'" );
		}
	});

	// start execution of the bot
	bot.start(dpp::st_wait);
	return;
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
static inline
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
	// user commands
	resource_man::register_global_slash_commands(command_list, bot);
	user_man::register_global_slash_commands(command_list, bot);

	// admin commands
	roles::register_global_slash_commands(command_list, bot);

	// new dynamic way:
	for(auto commandFp : Commands::CommandFunctionPointers::register_global_slash_commands()){
		Commands::call_register_global_slash_commands(commandFp, command_list, bot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
static inline
auto handle_global_slash_commands(
	const dpp::slashcommand_t& event, 
	dpp::cluster& bot, 
	const std::vector<dpp::slashcommand>& command_list
) -> void {

	// user commands
	resource_man::handle_global_slash_commands(event, bot);
	user_man::handle_global_slash_commands(event, bot);

	// admin commands
	roles::handle_global_slash_commands(event, bot);

	// new dynamic way:
	for(auto commandFp : Commands::CommandFunctionPointers::handle_global_slash_commands()){
		Commands::call_handle_global_slash_commands(commandFp, event, bot, command_list);
	}
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
static inline
auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void {
	for(auto commandFp : Commands::CommandFunctionPointers::handle_button_clicks()){
		Commands::call_handle_button_clicks(commandFp, event, bot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE FORM SUBMITS
//////////////////////////////////////////////////////////////////////////////
static inline
auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot) -> void {
	for(auto commandFp : Commands::CommandFunctionPointers::handle_form_submits()){
		Commands::call_handle_form_submits(commandFp, event, bot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ADDED REACTIONS
//////////////////////////////////////////////////////////////////////////////
static inline
auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void {
	roles::handle_reaction_added(event, bot);

	// new dynamic way:
	for(auto commandFp : Commands::CommandFunctionPointers::handle_reaction_added()){
		Commands::call_handle_reaction_added(commandFp, event, bot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE REMOVED REACTIONS
//////////////////////////////////////////////////////////////////////////////
static inline
auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void {
	roles::handle_reaction_removed(event, bot);

	for(auto commandFp : Commands::CommandFunctionPointers::handle_reaction_removed()){
		Commands::call_handle_reaction_removed(commandFp, event, bot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// WELCOME NEW MEMBERS TO A GUILD
//////////////////////////////////////////////////////////////////////////////
static inline
auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void {
	user_man::welcome_member(event, bot);

	// new dynamic way:
	for(auto commandFp : Commands::CommandFunctionPointers::welcome_member()){
		Commands::call_welcome_member(commandFp, event, bot);
	}
}

//////////////////////////////////////////////////////////////////////////////
// REMOVE MEMBERS FROM A GUILD
//////////////////////////////////////////////////////////////////////////////
static inline
auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void {
	user_man::leave_member(event, bot);

	// new dynamic way:
	for(auto commandFp : Commands::CommandFunctionPointers::leave_member()){
		Commands::call_leave_member(commandFp, event, bot);
	}
}