#include <dpp/appcommand.h>
#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
//////////////////////////////////////////////////////////////////////////////
auto main() -> int {
	// initialize bot
	dpp::cluster bot(read_bot_token("bot_token.txt"));
	bot.on_log(dpp::utility::cout_logger());

	// list of slash commands
	std::vector<dpp::slashcommand> global_command_list;
	register_global_slash_commands(global_command_list);

	// register slash commands
	bot.on_ready([&bot, &global_command_list](const dpp::ready_t& event) {
		if(dpp::run_once<struct register_bot_commands>()) {
			for(auto& command : global_command_list) { 
				bot.global_command_create(
					command	
				);
			}
		}
	});

	// handle slash commands
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
			handle_global_slash_commands(event);
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
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list) -> void {

}

//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
auto handle_global_slash_commands(const dpp::slashcommand_t& event) -> void {

}