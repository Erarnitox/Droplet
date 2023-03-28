#include "main.hpp"
#include "resource_man.hpp"
#include "user_man.hpp"
#include <dpp/dispatcher.h>

//////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////////
auto main() -> int {
	// initialize bot
	dpp::cluster bot(read_bot_token("bot_token.txt"));
	bot.on_log(dpp::utility::cout_logger());

	// list of slash commands
	std::vector<dpp::slashcommand> global_command_list;
	register_global_slash_commands(global_command_list, bot);

	// when a member joins the guild
	bot.on_guild_member_add([&bot](const dpp::guild_member_add_t& event) -> void {
		welcome_member(event, bot);
	});

	// when a member leaves the guild
	bot.on_guild_member_remove([&bot](const dpp::guild_member_remove_t& event) -> void {
		leave_member(event, bot);
	});

	// register slash commands
	bot.on_ready([&bot, &global_command_list](const dpp::ready_t& event) -> void {
		if(dpp::run_once<struct register_bot_commands>()) {
			bot.global_bulk_command_create(global_command_list);
		}
	});

	// handle slash commands
	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) -> void {
			handle_global_slash_commands(event, bot);
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
		throw("ERROR: bot token file could not be opened!");
	}

	if(bot_token.size() < 1)
		throw("ERROR: NO bot token found!");

	return bot_token;
}

//////////////////////////////////////////////////////////////////////////////
// RGISTER ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
	resource_man::register_global_slash_commands(command_list, bot);
	core::register_global_slash_commands(command_list, bot);
}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
	resource_man::handle_global_slash_commands(event);
	core::handle_global_slash_commands(event, bot);
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