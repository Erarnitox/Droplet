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

#include "RemoveNotificationCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <Core.hpp>
#include <NotificationRepository.hpp>
#include <format>

#include "Bot.hpp"
#include "LatestEventsRepository.hpp"
#include "SetNotificationCommand.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
RemoveNotificationCommand::RemoveNotificationCommand() : IGlobalSlashCommand() {
	this->command_name = "remove_notification";
	this->command_description = "Remove notification events from this server (Admin only!)";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void RemoveNotificationCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};

	NotificationRepository repo;
	const auto existing{repo.get(guild_id)};

	if (existing.channel_id != 0) {
		if (repo.remove(guild_id)) {
			if (existing.type == "youtube" && not existing.data.empty()) {
				const auto key{std::format("{}/{}", existing.channel_id, existing.data)};
				stop_youtube_notification_daemon(key);
				(void)LatestEventsRepository::remove(key);
			}
			Core::timed_reply_private(*Bot::ctx, event, "Notifications where removed!", 2000);
		} else {
			Core::timed_reply_private(*Bot::ctx, event, "Error: Failed to remove the Notifications!", 2000);
		}
	} else {
		Core::timed_reply_private(*Bot::ctx, event, "There are no Notifications on the Server!", 2000);
	}
	return;
}
