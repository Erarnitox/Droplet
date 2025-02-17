#include "RemoveNotificationCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <Core.hpp>
#include <NotificationRepository.hpp>

RemoveNotificationCommand::RemoveNotificationCommand() : IGlobalSlashCommand() {
	this->command_name = "remove_notification";
	this->command_description = "Remove notification events from this server (Admin only!)";
}

void RemoveNotificationCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};
	const auto& channel_id{static_cast<size_t>(cmd.channel_id)};

	NotificationRepository repo;
	NotificationDTO data{guild_id, channel_id, {}, {}, {}};

	if (repo.get(data.guild_id).channel_id != 0) {
		if (repo.remove(data.guild_id)) {
			auto msg{dpp::message("Notifications where removed!")};
			event.reply(msg);
		} else {
			event.reply(dpp::message("Error: Failed to remove the Notifications!").set_flags(dpp::m_ephemeral));
		}
	} else {
		auto msg{dpp::message("There are no Notifications on the Server!")};
		event.reply(msg);
	}
	return;
}