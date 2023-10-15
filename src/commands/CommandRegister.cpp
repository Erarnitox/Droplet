#include "CommandRegister.hpp"

///////////////////////////////////////
/// COMMAND HEADERS:
///////////////////////////////////////
#include "ChallengeRoleCommand.hpp"
#include "Commands.hpp"
#include "HelpCommand.hpp"
#include "SetChannelCommand.hpp"

void CommandRegister::registerCommands() {
	////////////////////////////////////
	/// REGISTER ALL COMMANDS HERE:
	////////////////////////////////////
	(void)Commands::CommandRegistration<ChallengeRoleCommand>();
	(void)Commands::CommandRegistration<HelpCommand>();
	(void)Commands::CommandRegistration<SetChannelCommand>();
}