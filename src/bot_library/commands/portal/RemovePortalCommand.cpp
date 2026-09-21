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

#include "RemovePortalCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <PortalIndex.hpp>
#include <PortalRepository.hpp>

//----------------------------------------
//
//----------------------------------------
RemovePortalCommand::RemovePortalCommand(AppContext& ctx) : db_(ctx.db), portal_index_(ctx.portal_index) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->admin_only = true;
}

//----------------------------------------
//
//----------------------------------------
void RemovePortalCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& cmd{event.command};
	const auto guild_id{static_cast<size_t>(cmd.guild_id)};

	PortalRepository repo{db_};
	const auto stored{repo.get(guild_id)};

	if (stored.channel_id != 0) {
		if (repo.remove(guild_id)) {
			portal_index_.remove(static_cast<std::uint64_t>(stored.channel_id));
			auto msg{dpp::message("Portal was removed!")};
			event.reply(msg);
		} else {
			event.reply(dpp::message("Error: Failed to remove the Portal!").set_flags(dpp::m_ephemeral));
		}
	} else {
		auto msg{dpp::message("There is no Portal on the Server!")};
		event.reply(msg);
	}
}
