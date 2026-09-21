#include <AddResourcesCommand.hpp>
#include <Bot.hpp>
#include <GetResourcesCommand.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_resource_commands(Bot& bot, AppContext& ctx) {
	bot.add_slash_command(std::make_shared<AddResourcesCommand>(ctx));
	bot.add_slash_command(std::make_shared<GetResourcesCommand>(ctx));
}
