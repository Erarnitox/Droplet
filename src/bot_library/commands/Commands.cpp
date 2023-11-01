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
// #include "HelpCommand.hpp"
// #include "SetChannelCommand.hpp"
#include <PongCommand.hpp>

/**
* @brief registers the commands so they are known to the bot
* @return doesn't return anything
*/
void Commands::registerCommands() {
	////////////////////////////////////
	/// REGISTER ALL COMMANDS HERE:
	////////////////////////////////////
	// TODO: Replace with new way to register commands
	//  (void)Commands::CommandRegistration<ChallengeRoleCommand>();
	//  (void)Commands::CommandRegistration<HelpCommand>();
	//  (void)Commands::CommandRegistration<SetChannelCommand>();

	// new way to register commands:
	Bot::add_message_command(std::make_shared<PongCommand>());
}