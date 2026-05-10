/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RegisterModules.hpp"

#include <AddResourcesCommand.hpp>
#include <Bot.hpp>
#include <GetResourcesCommand.hpp>
#include <memory>

void register_resource_commands() {
	Bot::add_slash_command(std::make_shared<AddResourcesCommand>());
	Bot::add_slash_command(std::make_shared<GetResourcesCommand>());
}
