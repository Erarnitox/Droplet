#include <Commands.hpp>

#include "registration/RegisterAllCommands.hpp"

void Commands::registerCommands(Bot& bot, AppContext& ctx) {
	CommandRegistration::register_all(bot, ctx);
}
