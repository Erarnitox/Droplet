#include "SetNotificationCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <cpr/cpr.h>
#include <dispatcher.h>
#include <message.h>
#include <queues.h>
#include <snowflake.h>

#include <Core.hpp>
#include <NotificationRepository.hpp>
#include <format>
#include <optional>
#include <regex>

#include "LatestEventsRepository.hpp"
#include "repositories/NotificationRepository.hpp"

static inline auto resolve_youtube_channel_id(const std::string& input) -> std::optional<std::string> {
	if(input.starts_with("UC")) return input;

	std::smatch match;
	std::regex channelRegex{R"(youtube\.com\/channel\/([a-zA-Z0-9_-]+))"};
	if (std::regex_search(input, match, channelRegex) && match.size() > 1) {
		return match[1].str();
	}

	std::regex userRegex{R"(youtube\.com\/user\/([a-zA-Z0-9_-]+))"};
	std::string username;
	if (std::regex_search(input, match, userRegex) && match.size() > 1) {
		username = match[1].str();
	}

	if(username.empty()) {
		std::regex handleRegex{R"(youtube\.com\/@([a-zA-Z0-9_-]+))"};
		if (std::regex_search(input, match, handleRegex) && match.size() > 1) {
			username = match[1].str();
		}
	}

	if(username.empty()) {
		username = input;
	}

	cpr::Response r = cpr::Get(cpr::Url{"https://youtube.com/@" + username});

	if (r.status_code != 200) {
		return std::nullopt;
	}

	if (std::regex_search(r.text, match, channelRegex) && match.size() > 1) {
		return match[1].str();
	} else {
		return std::nullopt;
	}
}

static inline auto start_notification_deamon(size_t channel_id,
											 const std::string& youtube_id,
											 const std::string& message,
											 size_t timestep_sec = 500) -> void {

	const auto key{std::format("{}/{}", channel_id, youtube_id)};

	const dpp::timer_callback_t on_tick{[channel_id, youtube_id, message, key](dpp::timer timer_handle) {
		LatestEventsRepository::set_active(key, true);
		const auto url{std::format("https://www.youtube.com/feeds/videos.xml?channel_id={}", youtube_id)};

		Bot::ctx->request(
			url,
			dpp::m_get,
			[channel_id, message, youtube_id, key, timer_handle](const dpp::http_request_completion_t& cc) {
				if (cc.status >= 300) {
					if (cc.status >= 400) {
						if (LatestEventsRepository::remove(key))
							Bot::ctx->stop_timer(timer_handle);
					}
					return;
				}

				if (not LatestEventsRepository::is_active(key)) {
					Bot::ctx->stop_timer(timer_handle);
					return;
				}

				static const std::regex video_regex{R"(https:\/\/www\.youtube\.com\/v\/([a-zA-Z0-9_-]+))"};
				std::smatch match;
				if (std::regex_search(cc.body, match, video_regex) && match.size() > 1) {
					std::string video_id = match[1].str();
					std::string yt_link = std::format("https://www.youtube.com/v/{}", video_id);
					if (LatestEventsRepository::exists(key, yt_link)) {
						return;
					}
					if (not LatestEventsRepository::insert(key, yt_link)) {
						return;
					}

					dpp::message msg(channel_id, std::format("{}\n{}", message, yt_link));
					Bot::ctx->message_create(msg);
				} else {
					Bot::ctx->stop_timer(timer_handle);
				}
			});
	}};

	Bot::ctx->start_timer(on_tick, timestep_sec);
}

SetNotificationCommand::SetNotificationCommand() : IGlobalSlashCommand(), IReady() {
	this->command_name = "youtube_uploads";
	this->command_description = "Get notifications about youtube uploads to discord (Admin only!)";

	this->command_options.emplace_back(dpp::co_channel, "channel", "In which channel to post the notifications", true);

	this->command_options.emplace_back(dpp::co_string, "youtube_id", "Youtube channel id", true);

	this->command_options.emplace_back(
		dpp::co_string, "message", "Supply a custom message that prepends the link", true);
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

	const auto youtube_opt{ resolve_youtube_channel_id(youtube_username) };
	if (youtube_opt.has_value()) {
		const auto& youtube_id{ youtube_opt.value() };

		NotificationRepository repo;
		NotificationDTO data{guild_id, channel_id, "youtube", youtube_id, message, 500};

		if (repo.create(data)) {
			constexpr size_t timestep_sec{ 500 };
			start_notification_deamon(channel_id, youtube_id, message, timestep_sec);
			Core::timed_reply_private(*Bot::ctx, event, "Notifications for youtube enabled!", 2000);
		} else {
			Core::timed_reply_private(*Bot::ctx, event, "Can't find the given youtube channel!", 2000);
		}
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
		if (job.type != "youtube")
			continue;
		start_notification_deamon(job.channel_id, job.data, job.message, job.timestep);
	}
}
