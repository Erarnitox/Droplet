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

#include "BlacklistCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <AppContext.hpp>
#include <BlacklistDTO.hpp>
#include <BlacklistRepository.hpp>
#include <Core.hpp>
#include <PortalIndex.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
BlacklistCommand::BlacklistCommand(AppContext& ctx)
	: discord_(ctx.discord), db_(ctx.db), blacklist_index_(ctx.blacklist_index) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);

	this->command_options.emplace_back(dpp::co_string, "username", "The username to be blacklisted", true);
	this->admin_only = true;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void BlacklistCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (not Core::is_erarnitox_admin(event.command.member)) {
		event.reply("Only Erarnitox's admins are allowed to run this command!");
		return;
	}

	const auto username{Core::get_parameter(discord_, event, "username")};
	if (username.empty() || blacklist_index_.contains(username)) {
		return;
	}

	BlacklistRepository repo{db_};
	BlacklistDTO dto;
	dto.username = username;
	if (repo.create(dto)) {
		blacklist_index_.add(username);
		event.reply(dpp::message("User was added to the blacklist!").set_flags(dpp::m_ephemeral));
	} else {
		event.reply(dpp::message("Something went wrong!").set_flags(dpp::m_ephemeral));
	}

	return;
}