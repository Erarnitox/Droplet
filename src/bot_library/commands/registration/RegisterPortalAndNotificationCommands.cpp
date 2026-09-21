#include <BlacklistCommand.hpp>
#include <Bot.hpp>
#include <RemoveNotificationCommand.hpp>
#include <RemovePortalCommand.hpp>
#include <SetPortalCommand.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_portal_and_notification_commands(Bot& bot, AppContext& ctx) {
	bot.add_slash_command(std::make_shared<RemovePortalCommand>(ctx));
	bot.add_slash_command(std::make_shared<BlacklistCommand>(ctx));
	bot.add_slash_command(std::make_shared<RemoveNotificationCommand>(ctx));

	const auto portal_command{std::make_shared<SetPortalCommand>(ctx)};
	bot.add_slash_command(portal_command);
	bot.add_message_command(portal_command);
}
