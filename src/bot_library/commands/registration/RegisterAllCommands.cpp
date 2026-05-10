/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RegisterAllCommands.hpp"

#include "RegisterModules.hpp"

namespace CommandRegistration {

void register_all() {
	register_essential_commands();
	register_slash_utility_commands();
	register_resource_commands();
	register_portal_and_notification_commands();
	register_interactive_commands();
}

}  // namespace CommandRegistration
