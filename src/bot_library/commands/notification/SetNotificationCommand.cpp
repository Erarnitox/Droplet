/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "SetNotificationCommand.hpp"

#include <appcommand.h>
#include <snowflake.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <NotificationRepository.hpp>

#include "YoutubeNotificationService.hpp"

SetNotificationCommand::SetNotificationCommand(AppContext& ctx)
	: discord_(ctx.discord), db_(ctx.db), youtube_(ctx.youtube) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->admin_only = true;
	this->command_options.emplace_back(dpp::co_channel, "channel", "In which channel to post the notifications", true);
	this->command_options.emplace_back(dpp::co_string, "youtube_channel", "Youtube channel", true);
	this->command_options.emplace_back(
		dpp::co_string, "message", "Supply a custom message that prepends the link", true);
}

void SetNotificationCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (not Core::is_admin(event.command.member)) {
		Core::timed_reply_private(discord_, event, "Only admins are allowed to use this command!", 2000);
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};
	const auto channel_id{std::get<dpp::snowflake>(event.get_parameter("channel"))};

	const auto youtube_username{Core::get_parameter(discord_, event, "youtube_channel")};
	if (youtube_username.empty()) {
		return;
	}

	const auto message{Core::get_parameter(discord_, event, "message")};
	if (message.empty()) {
		return;
	}

	const auto youtube_opt{resolve_youtube_channel(youtube_username)};
	if (youtube_opt.has_value()) {
		const auto& youtube_id{youtube_opt.value()};
		const bool allow_broadcast{Core::is_erarnitox_admin(event.command.member)};
		NotificationRepository repo{db_};
		const NotificationDTO data{guild_id, channel_id, "youtube", youtube_id, message, 500, allow_broadcast};

		if (repo.create(data)) {
			constexpr size_t timestep_sec{500};
			youtube_.start_job(channel_id, youtube_id, message, timestep_sec, allow_broadcast);
			Core::timed_reply_private(discord_, event, "Notifications for youtube enabled!", 2000);
		} else {
			Core::timed_reply_private(discord_, event, "Can't find the given youtube channel!", 2000);
		}
	} else {
		Core::timed_reply_private(discord_, event, "Error: Failed to enable Upload notifications!", 2000);
	}
}

void SetNotificationCommand::on_ready(const dpp::ready_t& event) {
	(void)event;
	if (youtube_.load_from_database()) {
		return;
	}

	// The database was not reachable right now: without a retry no YouTube notification would
	// ever be started again until the next restart. Retry every 30s; the actual work runs on the
	// library's worker pool so that the event-loop thread never blocks on the database.
	if (load_retry_pending_.exchange(true)) {
		return;
	}
	discord_.log(dpp::ll_warning, "YouTube notifications could not be loaded (database unavailable), retrying in 30s");
	(void)discord_.start_timer(
		[this](dpp::timer timer_handle) {
			discord_.queue_work(0, [this, timer_handle] {
				try {
					if (youtube_.load_from_database()) {
						discord_.stop_timer(timer_handle);
						load_retry_pending_ = false;
					}
				} catch (...) {
				}
			});
		},
		30);
}
