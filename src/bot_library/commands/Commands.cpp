#include <Commands.hpp>

///////////////////////////////////////
/// COMMAND HEADERS:
///////////////////////////////////////
// #include "ChallengeRoleCommand.hpp"
// #include "HelpCommand.hpp"
// #include "SetChannelCommand.hpp"
// #include <PongCommand.hpp>

void CommandRegister::registerCommands() {
	////////////////////////////////////
	/// REGISTER ALL COMMANDS HERE:
	////////////////////////////////////
	// TODO: Replace with new way to register commands
	//  (void)Commands::CommandRegistration<ChallengeRoleCommand>();
	//  (void)Commands::CommandRegistration<HelpCommand>();
	//  (void)Commands::CommandRegistration<SetChannelCommand>();

	// new way to register commands:
	// Bot::add_message_command(std::make_shared<PongCommand>());
}