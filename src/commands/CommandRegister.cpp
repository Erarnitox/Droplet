#include "CommandRegister.hpp"

///////////////////////////////////////
/// COMMAND HEADERS:
///////////////////////////////////////
#include "ChallengeRoleCommand.hpp"

void CommandRegister::registerCommands() {
    ////////////////////////////////////
    /// REGISTER ALL COMMANDS HERE:
    ////////////////////////////////////
    (void) Commands::CommandRegistration<ChallengeRoleCommand>();
}