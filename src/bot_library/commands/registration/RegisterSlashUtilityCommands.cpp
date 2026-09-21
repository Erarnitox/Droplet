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

void register_slash_utility_commands(Bot& bot, AppContext& ctx) {
	bot.add_slash_command(std::make_shared<HelpCommand>(ctx));
	bot.add_slash_command(std::make_shared<ServerDetails>(ctx));
	bot.add_slash_command(std::make_shared<InviteCommand>(ctx));
	bot.add_slash_command(std::make_shared<LearnCPPCommand>(ctx));
	bot.add_slash_command(std::make_shared<RollCommand>(ctx));
	bot.add_slash_command(std::make_shared<FlipCommand>(ctx));
	bot.add_slash_command(std::make_shared<ProfileCommand>(ctx));
	bot.add_slash_command(std::make_shared<ReminderCommand>(ctx));
	bot.add_slash_command(std::make_shared<ClearCommand>(ctx));
	bot.add_slash_command(std::make_shared<WarnCommand>(ctx));
	bot.add_slash_command(std::make_shared<LeaderboardCommand>(ctx));
}
