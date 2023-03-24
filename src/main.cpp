#include "main.hpp"
#include "resource_man.hpp"

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

	// register slash commands
	bot.on_ready([&bot, &global_command_list](const dpp::ready_t& event) {
		if(dpp::run_once<struct register_bot_commands>()) {
			bot.global_bulk_command_create(global_command_list);
		}
	});

	// handle slash commands
	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
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
auto handle_global_slash_commands(const dpp::slashcommand_t& event, const dpp::cluster& bot) -> void {
	resource_man::handle_global_slash_commands(event);
}