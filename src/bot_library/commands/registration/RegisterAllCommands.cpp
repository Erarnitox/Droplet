#include "RegisterAllCommands.hpp"

#include "RegisterModules.hpp"

namespace CommandRegistration {

void register_all(Bot& bot, AppContext& ctx) {
	register_essential_commands(bot, ctx);
	register_slash_utility_commands(bot, ctx);
	register_resource_commands(bot, ctx);
	register_portal_and_notification_commands(bot, ctx);
	register_interactive_commands(bot, ctx);
}

}  // namespace CommandRegistration
