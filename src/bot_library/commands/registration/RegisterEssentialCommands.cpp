#include <Bot.hpp>
#include <CleanUp.hpp>
#include <LearnCppTutorialHint.hpp>
#include <SetNotificationCommand.hpp>
#include <SetStatus.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_essential_commands(Bot& bot, AppContext& ctx) {
	bot.add_ready_command(std::make_shared<SetStatus>(ctx));
	const auto notifications{std::make_shared<SetNotificationCommand>(ctx)};
	bot.add_ready_command(notifications);
	bot.add_slash_command(notifications);
	bot.add_message_delete_command(std::make_shared<CleanUp>(ctx));
	bot.add_message_command(std::make_shared<LearnCppTutorialHint>(ctx));
}
