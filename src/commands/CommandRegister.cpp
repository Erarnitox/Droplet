#include "CommandRegister.hpp"

///////////////////////////////////////
/// COMMAND HEADERS:
///////////////////////////////////////
#include "Commands.hpp"
#include "ChallengeRoleCommand.hpp"
#include "HelpCommand.hpp"
#include "SetChannelCommand.hpp"

void CommandRegister::registerCommands() {
    ////////////////////////////////////
    /// REGISTER ALL COMMANDS HERE:
    ////////////////////////////////////
    (void) Commands::CommandRegistration<ChallengeRoleCommand>();
    (void) Commands::CommandRegistration<HelpCommand>();
    (void) Commands::CommandRegistration<SetChannelCommand>();
}