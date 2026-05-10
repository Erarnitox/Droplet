/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RegisterModules.hpp"

#include <Bot.hpp>
#include <CleanUp.hpp>
#include <SetNotificationCommand.hpp>
#include <SetStatus.hpp>
#include <memory>

void register_essential_commands() {
	Bot::add_ready_command(std::make_shared<SetStatus>());
	Bot::add_ready_command(std::make_shared<SetNotificationCommand>());
	Bot::add_message_command(std::make_shared<CleanUp>());
}
