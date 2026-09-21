/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "Bot.hpp"

#include <appcommand.h>
#include <dpp/cluster.h>
#include <intents.h>

#include "BotEventSubscriptions.hpp"

Bot::Bot(const std::string& token) {
	const auto intents{dpp::i_default_intents | dpp::i_message_content | dpp::i_guild_messages};
	ctx = std::make_unique<dpp::cluster>(token, intents);
}

void Bot::add_slash_command(const std::shared_ptr<IGlobalSlashCommand>& slash_command) {
	slash_commands[slash_command->command_name] = slash_command;
}

void Bot::add_button_command(const std::shared_ptr<IButtonCommand>& button_command) {
	button_commands.push_back(button_command);
}

void Bot::add_form_command(const std::shared_ptr<IFormCommand>& form_command) {
	form_commands.push_back(form_command);
}

void Bot::add_message_command(const std::shared_ptr<IMessageCommand>& message_command) {
	message_commands.push_back(message_command);
}

void Bot::add_message_delete_command(const std::shared_ptr<IMessageCommand>& message_command) {
	message_delete_commands.push_back(message_command);
}

void Bot::add_reaction_command(const std::shared_ptr<IReactionCommand>& reaction_command) {
	reaction_commands.push_back(reaction_command);
}

void Bot::add_ready_command(const std::shared_ptr<IReady>& ready_command) {
	ready_commands.push_back(ready_command);
}

void Bot::run() {
	subscribe_bot_event_handlers(*this);
	ctx->start(dpp::st_wait);
}

void Bot::shutdown() noexcept {
	ctx.reset();
	slash_commands.clear();
	button_commands.clear();
	form_commands.clear();
	message_commands.clear();
	message_delete_commands.clear();
	reaction_commands.clear();
	ready_commands.clear();
}
