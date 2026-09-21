/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RemoveNotificationCommand.hpp"

#include <message.h>
#include <snowflake.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <NotificationRepository.hpp>
#include <format>

#include "YoutubeNotificationService.hpp"

RemoveNotificationCommand::RemoveNotificationCommand(AppContext& ctx)
	: discord_(ctx.discord), db_(ctx.db), youtube_(ctx.youtube) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->admin_only = true;
}

void RemoveNotificationCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};

	NotificationRepository repo{db_};
	const auto existing{repo.get(guild_id)};

	if (existing.channel_id != 0) {
		if (repo.remove(guild_id)) {
			if (existing.type == "youtube" && not existing.data.empty()) {
				const auto key{std::format("{}/{}", existing.channel_id, existing.data)};
				youtube_.stop_job(key);
			}
			Core::timed_reply_private(discord_, event, "Notifications where removed!", 2000);
		} else {
			Core::timed_reply_private(discord_, event, "Error: Failed to remove the Notifications!", 2000);
		}
	} else {
		Core::timed_reply_private(discord_, event, "There are no Notifications on the Server!", 2000);
	}
}
