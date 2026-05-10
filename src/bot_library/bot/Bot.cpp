/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class handles the execution of the
 *	bot and delegats events to registerd commands.
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include "Bot.hpp"

#include "BotEventSubscriptions.hpp"

#include <appcommand.h>
#include <dpp/cluster.h>
#include <intents.h>

#include <memory>

ctx_t Bot::ctx;
button_commands_t Bot::button_commands;
form_commands_t Bot::form_commands;
slash_commands_t Bot::slash_commands;
member_commands_t Bot::member_commands;
message_commands_t Bot::message_commands;
reaction_commands_t Bot::reaction_commands;
ready_commands_t Bot::ready_commands;

void Bot::init(const std::string& token) {
	const auto intents{dpp::i_default_intents | dpp::i_message_content | dpp::i_guild_messages};
	Bot::ctx = std::make_unique<dpp::cluster>(token, intents);

	Bot::button_commands = button_commands_t();
	Bot::form_commands = form_commands_t();
	Bot::slash_commands = slash_commands_t();
	Bot::member_commands = member_commands_t();
	Bot::message_commands = message_commands_t();
	Bot::reaction_commands = reaction_commands_t();
	Bot::ready_commands = ready_commands_t();
}

void Bot::add_slash_command(const std::shared_ptr<IGlobalSlashCommand>& slash_command) {
	Bot::slash_commands[slash_command->command_name] = slash_command;
}

void Bot::add_button_command(const std::shared_ptr<IButtonCommand>& button_command) {
	Bot::button_commands.push_back(button_command);
}

void Bot::add_form_command(const std::shared_ptr<IFormCommand>& form_command) {
	Bot::form_commands.push_back(form_command);
}

void Bot::add_member_command(const std::shared_ptr<IMemberCommand>& member_command) {
	Bot::member_commands.push_back(member_command);
}

void Bot::add_message_command(const std::shared_ptr<IMessageCommand>& message_command) {
	Bot::message_commands.push_back(message_command);
}

void Bot::add_reaction_command(const std::shared_ptr<IReactionCommand>& reaction_command) {
	Bot::reaction_commands.push_back(reaction_command);
}

void Bot::add_ready_command(const std::shared_ptr<IReady>& ready_command) {
	Bot::ready_commands.push_back(ready_command);
}

void Bot::run() {
	subscribe_bot_event_handlers(ctx);
	Bot::ctx->start(dpp::st_wait);
}
