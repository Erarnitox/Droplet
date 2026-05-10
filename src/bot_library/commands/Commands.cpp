/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class manages all commands and their registration
 *	so the bot knows about it.
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include <Commands.hpp>

#include "registration/RegisterAllCommands.hpp"

void Commands::registerCommands() {
	CommandRegistration::register_all();
}
