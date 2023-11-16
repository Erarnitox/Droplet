/**
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class manages all commands and their registration
 *	so the bot knows about it.
 *
 *  Documentation: https://droplet.dropsoft.org/doxygen/html/commands
 **/

#include <Commands.hpp>

///////////////////////////////////////
/// COMMAND HEADERS:
///////////////////////////////////////
// #include "ChallengeRoleCommand.hpp"
// #include "SetChannelCommand.hpp"

#include <ChallengeRoleCommand.hpp>
#include <HelpCommand.hpp>
#include <PongCommand.hpp>
#include <SetChannelCommand.hpp>
#include <memory>

/**
 * @brief registers the commands so they are known to the bot
 * @return doesn't return anything
 */
void Commands::registerCommands() {
	////////////////////////////////////
	/// REGISTER ALL COMMANDS HERE:
	////////////////////////////////////
	Bot::add_message_command(std::make_shared<PongCommand>());
	Bot::add_slash_command(std::make_shared<HelpCommand>());
	Bot::add_slash_command(std::make_shared<SetChannelCommand>());

	Bot::add_slash_command(std::make_shared<ChallengeRoleCommand>());
	Bot::add_button_command(std::make_shared<ChallengeRoleCommand>());
	Bot::add_form_command(std::make_shared<ChallengeRoleCommand>());
}