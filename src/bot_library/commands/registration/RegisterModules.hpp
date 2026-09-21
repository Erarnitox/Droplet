#pragma once

#include <AppContext.hpp>
#include <Bot.hpp>

void register_essential_commands(Bot& bot, AppContext& ctx);
void register_slash_utility_commands(Bot& bot, AppContext& ctx);
void register_resource_commands(Bot& bot, AppContext& ctx);
void register_portal_and_notification_commands(Bot& bot, AppContext& ctx);
void register_interactive_commands(Bot& bot, AppContext& ctx);
