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

#include "WarnCommand.hpp"

#include <message.h>
#include <role.h>

#include <AppContext.hpp>
#include <Core.hpp>

//----------------------------------------
//
//----------------------------------------
WarnCommand::WarnCommand(AppContext& ctx) : discord_(ctx.discord) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->command_options.emplace_back(dpp::co_user, "user", "User to warn", true);
}

//----------------------------------------
//
//----------------------------------------
void WarnCommand::warn_user(const dpp::slashcommand_t& event,
							const dpp::snowflake guild_id,
							const dpp::snowflake user_id,
							const dpp::role& warned_role) {
	const auto role_id{warned_role.id};

	discord_.guild_get_member(guild_id, user_id, [this, event, role_id](const dpp::confirmation_callback_t& cc_member) {
		if (cc_member.is_error()) {
			event.reply("Couldn't get user details!");
			return;
		}

		const auto member{std::get<dpp::guild_member>(cc_member.value)};
		const auto username{member.get_mention()};
		const auto member_roles{member.get_roles()};

		if (std::find(member_roles.begin(), member_roles.end(), role_id) != member_roles.end()) {
			discord_.guild_ban_add(member.guild_id, member.user_id);
			event.reply(username + " has been banned from the server for repeated violation of rules");
		} else {
			discord_.guild_member_add_role(member.guild_id, member.user_id, role_id);
			event.reply(username + " has been warned!");
		}
	});
}

//----------------------------------------
//
//----------------------------------------
void WarnCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& guild_id{event.command.guild_id};
	const auto user_id{std::get<dpp::snowflake>(event.get_parameter("user"))};

	discord_.roles_get(guild_id, [this, event, guild_id, user_id](const dpp::confirmation_callback_t& cc_roles) {
		if (cc_roles.is_error()) {
			event.reply("Failed to obtain roles");
			return;
		}

		const auto roles{std::get<dpp::role_map>(cc_roles.value)};
		std::optional<dpp::role> warned_role{std::nullopt};

		for (const auto& [id, role] : roles) {
			if (role.name == "warned") {
				warned_role = role;
				break;
			}
		}

		if (not warned_role.has_value()) {
			// create the warned role
			dpp::role new_role;
			new_role.name = "warned";
			new_role.guild_id = guild_id;
			new_role.colour = 0xFFAA00;
			new_role.permissions = 0;
			discord_.role_create(new_role,
								 [this, event, guild_id, user_id](const dpp::confirmation_callback_t& callback) {
									 if (callback.is_error()) {
										 event.reply("Something went wrong!");
										 return;
									 }

									 warn_user(event, guild_id, user_id, callback.get<dpp::role>());
								 });
			return;
		} else {
			warn_user(event, guild_id, user_id, warned_role.value());
		}
	});
}
