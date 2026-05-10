/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include <BlacklistCommand.hpp>
#include <Bot.hpp>
#include <RemoveNotificationCommand.hpp>
#include <RemovePortalCommand.hpp>
#include <SetNotificationCommand.hpp>
#include <SetPortalCommand.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_portal_and_notification_commands() {
	Bot::add_slash_command(std::make_shared<RemovePortalCommand>());
	Bot::add_slash_command(std::make_shared<BlacklistCommand>());
	Bot::add_slash_command(std::make_shared<SetNotificationCommand>());
	Bot::add_slash_command(std::make_shared<RemoveNotificationCommand>());

	const auto& portal_command{std::make_shared<SetPortalCommand>()};
	Bot::add_slash_command(portal_command);
	Bot::add_message_command(portal_command);
	Bot::add_reaction_command(portal_command);
}
