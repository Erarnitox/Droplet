/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include <Bot.hpp>
#include <ClearCommand.hpp>
#include <FlipCommand.hpp>
#include <HelpCommand.hpp>
#include <InviteCommand.hpp>
#include <LeaderboardCommand.hpp>
#include <LearnCPPCommand.hpp>
#include <ProfileCommand.hpp>
#include <ReminderCommand.hpp>
#include <RollCommand.hpp>
#include <ServerDetails.hpp>
#include <WarnCommand.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_slash_utility_commands() {
	Bot::add_slash_command(std::make_shared<HelpCommand>());
	Bot::add_slash_command(std::make_shared<ServerDetails>());
	Bot::add_slash_command(std::make_shared<InviteCommand>());
	Bot::add_slash_command(std::make_shared<LearnCPPCommand>());
	Bot::add_slash_command(std::make_shared<RollCommand>());
	Bot::add_slash_command(std::make_shared<FlipCommand>());
	Bot::add_slash_command(std::make_shared<ProfileCommand>());
	Bot::add_slash_command(std::make_shared<ReminderCommand>());
	Bot::add_slash_command(std::make_shared<ClearCommand>());
	Bot::add_slash_command(std::make_shared<WarnCommand>());
	Bot::add_slash_command(std::make_shared<LeaderboardCommand>());
}
