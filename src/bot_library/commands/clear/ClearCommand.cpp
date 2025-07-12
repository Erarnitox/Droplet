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

//-----------------------------------------------------
//
//-----------------------------------------------------
ClearCommand::ClearCommand() : IGlobalSlashCommand() {
	this->command_name = "clear";
	this->command_description = "clear the current channel completely";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void ClearCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name)
		return;

	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	event.reply("Channel will be cleaned shortly...");

	Bot::ctx->channel_delete(event.command.channel_id);
	Bot::ctx->channel_create(event.command.channel);
}
