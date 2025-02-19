#include "SetNotificationCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>
#include <message.h>
#include <queues.h>
#include <snowflake.h>

#include <Core.hpp>
#include <NotificationRepository.hpp>
#include <format>
#include <nlohmann/json.hpp>

#include "LatestEventsRepository.hpp"
#include "repositories/NotificationRepository.hpp"

static inline auto start_notification_deamon(size_t channel_id,
											 const std::string& youtube_user,
											 const std::string& message,
											 size_t timestep_sec = 100) -> void {
	const dpp::timer_callback_t on_tick = [channel_id, youtube_user, message](dpp::timer deleteTimer) {
		(void)deleteTimer;
		const auto& youtube_id = youtube_user;	// TODO: resolve the id automatically
		Bot::ctx->request(
			std::format("https://www.youtube.com/feeds/videos.xml?channel_id={}", youtube_id),
			dpp::m_get,
			[channel_id, message, youtube_id, &deleteTimer](const dpp::http_request_completion_t& cc) {
				const auto& key{std::format("{}/{}", channel_id, youtube_id)};

				if (cc.status > 300) {
					if(cc.status > 399)
						(void) LatestEventsRepository::remove(key);
					
					const auto& msg{dpp::message(channel_id, "Received Server error from youtube!")};
					Bot::ctx->message_create(msg);
					Bot::ctx->stop_timer(deleteTimer);
					return;
				}
				
				if (not LatestEventsRepository::is_active(key)) {
					const auto& msg{dpp::message(channel_id, "Notifications not active!")};
					Bot::ctx->message_create(msg);
					Bot::ctx->stop_timer(deleteTimer);
					return;
				}

				const auto video_pattern{std::string("https://www.youtube.com/v/")};
				const auto vid_id_size{11};
				const auto yt_link{cc.body.substr(cc.body.find(video_pattern), video_pattern.size() + vid_id_size)};

				if (LatestEventsRepository::exists(key, yt_link)){
					const auto& msg{dpp::message(channel_id, "No new videos!")};
					Bot::ctx->message_create(msg);
					return;
				}

				if (not LatestEventsRepository::insert(key, yt_link)){
					const auto& msg{dpp::message(channel_id, "Could not update the database!")};
					Bot::ctx->message_create(msg);
					return;
				}

				const auto& msg{dpp::message(channel_id, std::format("{}\n{}", message, yt_link))};
				Bot::ctx->message_create(msg);
			});
	};

	Bot::ctx->start_timer(on_tick, timestep_sec);
}

SetNotificationCommand::SetNotificationCommand() : IGlobalSlashCommand(), IReady() {
	this->command_name = "youtube_uploads";
	this->command_description = "Get notifications about youtube uploads to discord (Admin only!)";

	this->command_options.emplace_back(dpp::co_channel, "channel", "In which channel to post the notifications", true);

	this->command_options.emplace_back(dpp::co_string, "youtube_id", "Youtube channel id", true);

	this->command_options.emplace_back(dpp::co_string, "message", "Supply a custom message that prepends the link", true);
}

void SetNotificationCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		Core::timed_reply_private(*Bot::ctx, event, "Only admins are allowed to use this command!", 2000);
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};
	const auto channel_id{std::get<dpp::snowflake>(event.get_parameter("channel"))};

	const auto youtube_username{Core::get_parameter(*Bot::ctx, event, "youtube_id")};
	if (youtube_username.empty()) {
		return;
	}

	const auto message{Core::get_parameter(*Bot::ctx, event, "message")};
	if (message.empty()) {
		return;
	}

	NotificationRepository repo;
	NotificationDTO data{guild_id, channel_id, "youtube", youtube_username, message, 500};

	if (repo.create(data)) {
		start_notification_deamon(channel_id, youtube_username, message);
		Core::timed_reply_private(*Bot::ctx, event, "Notifications for youtube enabled!", 2000);
	} else {
		Core::timed_reply_private(*Bot::ctx, event, "Error: Failed to enable Upload notifications!", 2000);
	}
}

void SetNotificationCommand::on_ready(const dpp::ready_t& event) {
	(void)event;

	NotificationRepository repo;

	if (not LatestEventsRepository::load())
		return;

	for (auto& job : repo.getAll()) {
		if (job.type != "youtube") continue;
		start_notification_deamon(job.channel_id, job.data, job.message, job.timestep);
	}
}
