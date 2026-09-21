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

#include "InviteCommand.hpp"

#include <invite.h>
#include <message.h>

#include <AppContext.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
InviteCommand::InviteCommand(AppContext& ctx) {
	(void)ctx;
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void InviteCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id,
							 "**Please click on the link below to join my server!**\nhttp://discord.erarnitox.de")
					.set_reference(event.command.id)
					.set_flags(dpp::m_ephemeral));

	return;
}