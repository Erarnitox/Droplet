/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include "ClearCommand.hpp"

#include <message.h>

#include <AppContext.hpp>
#include <Core.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
ClearCommand::ClearCommand(AppContext& ctx) : discord_(ctx.discord) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->admin_only = true;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void ClearCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	event.reply("Channel will be cleaned shortly...");

	discord_.channel_delete(event.command.channel_id);
	discord_.channel_create(event.command.channel);
}
