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
#include <mutex>
#include <optional>
#include <regex>
#include <unordered_map>

#include "LatestEventsRepository.hpp"
#include "repositories/NotificationRepository.hpp"

namespace {

std::mutex youtube_daemon_mutex;
std::unordered_map<std::string, dpp::timer> youtube_daemon_timers;

struct YoutubeFeedEntry {
	std::string video_id;
	std::string title;
};

//-----------------------------------------------------
//
//-----------------------------------------------------
[[nodiscard]] std::optional<YoutubeFeedEntry> parse_latest_youtube_entry(const std::string& atom_body) {
	static const std::regex entry_regex{R"(<entry>([\s\S]*?)</entry>)"};
	std::smatch entry_match;
	if (not std::regex_search(atom_body, entry_match, entry_regex) || entry_match.size() < 2) {
		return std::nullopt;
	}

	const std::string entry{entry_match[1].str()};

	static const std::regex video_id_regex{R"(<yt:videoId>([^<]+)</yt:videoId>)"};
	std::smatch video_match;
	if (not std::regex_search(entry, video_match, video_id_regex) || video_match.size() < 2) {
		return std::nullopt;
	}

	YoutubeFeedEntry parsed;
	parsed.video_id = video_match[1].str();

	static const std::regex title_regex{R"(<title[^>]*>([^<]*)</title>)"};
	std::smatch title_match;
	if (std::regex_search(entry, title_match, title_regex) && title_match.size() > 1) {
		parsed.title = title_match[1].str();
	}

	return parsed;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
[[nodiscard]] std::optional<std::string> resolve_youtube_channel_id(const std::string& input) {
	if (input.starts_with("UC"))
		return input;

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

	if (username.empty()) {
		std::regex handleRegex{R"(youtube\.com\/@([a-zA-Z0-9_-]+))"};
		if (std::regex_search(input, match, handleRegex) && match.size() > 1) {
			username = match[1].str();
		}
	}

	if (username.empty()) {
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

//-----------------------------------------------------
//
//-----------------------------------------------------
void start_notification_deamon(size_t channel_id,
							   const std::string& youtube_id,
							   const std::string& message,
							   size_t timestep_sec = 500) {
	const auto key{std::format("{}/{}", channel_id, youtube_id)};

	const dpp::timer_callback_t on_tick{[channel_id, youtube_id, message, key](dpp::timer timer_handle) {
		LatestEventsRepository::set_active(key, true);
		const auto url{std::format("https://www.youtube.com/feeds/videos.xml?channel_id={}", youtube_id)};

		Bot::ctx->request(
			url, dpp::m_get, [channel_id, message, key, timer_handle](const dpp::http_request_completion_t& cc) {
				const auto stop_timer_and_release = [key, timer_handle]() {
					Bot::ctx->stop_timer(timer_handle);
					std::lock_guard<std::mutex> lock(youtube_daemon_mutex);
					const auto it{youtube_daemon_timers.find(key)};
					if (it != youtube_daemon_timers.end() && it->second == timer_handle) {
						youtube_daemon_timers.erase(it);
					}
				};

				// Keep announced history on errors. Retry 5xx; stop timer on hard 4xx.
				if (cc.status >= 500) {
					return;
				}
				if (cc.status >= 400) {
					stop_timer_and_release();
					return;
				}
				if (cc.status >= 300) {
					return;
				}

				if (not LatestEventsRepository::is_active(key)) {
					stop_timer_and_release();
					return;
				}

				const auto entry_opt{parse_latest_youtube_entry(cc.body)};
				if (not entry_opt.has_value()) {
					stop_timer_and_release();
					return;
				}

				const auto& entry = entry_opt.value();
				const auto claim{LatestEventsRepository::try_claim_video(key, entry.video_id, entry.title)};
				if (claim != YoutubeClaimResult::Claimed) {
					return;
				}

				const auto yt_link{std::format("https://www.youtube.com/watch?v={}", entry.video_id)};
				dpp::message msg(channel_id, std::format("{}\n{}", message, yt_link));
				msg.set_allowed_mentions(true, true, true, true);
				Bot::ctx->message_create(msg);
			});
	}};

	std::lock_guard<std::mutex> lock(youtube_daemon_mutex);
	if (const auto it{youtube_daemon_timers.find(key)}; it != youtube_daemon_timers.end()) {
		Bot::ctx->stop_timer(it->second);
		youtube_daemon_timers.erase(it);
	}
	const dpp::timer new_handle{Bot::ctx->start_timer(on_tick, timestep_sec)};
	youtube_daemon_timers.emplace(key, new_handle);
}

}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
void stop_youtube_notification_daemon(const std::string& key) {
	LatestEventsRepository::set_active(key, false);

	std::lock_guard<std::mutex> lock(youtube_daemon_mutex);
	const auto it{youtube_daemon_timers.find(key)};
	if (it == youtube_daemon_timers.end()) {
		return;
	}
	Bot::ctx->stop_timer(it->second);
	youtube_daemon_timers.erase(it);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
SetNotificationCommand::SetNotificationCommand() : IGlobalSlashCommand(), IReady() {
	this->command_name = "youtube_uploads";
	this->command_description = "Get notifications about youtube uploads to discord (Admin only!)";

	this->command_options.emplace_back(dpp::co_channel, "channel", "In which channel to post the notifications", true);

	this->command_options.emplace_back(dpp::co_string, "youtube_channel", "Youtube channel", true);

	this->command_options.emplace_back(
		dpp::co_string, "message", "Supply a custom message that prepends the link", true);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetNotificationCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (not Core::is_admin(event.command.member)) {
		Core::timed_reply_private(*Bot::ctx, event, "Only admins are allowed to use this command!", 2000);
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};
	const auto channel_id{std::get<dpp::snowflake>(event.get_parameter("channel"))};

	const auto youtube_username{Core::get_parameter(*Bot::ctx, event, "youtube_channel")};
	if (youtube_username.empty()) {
		return;
	}

	const auto message{Core::get_parameter(*Bot::ctx, event, "message")};
	if (message.empty()) {
		return;
	}

	const auto youtube_opt{resolve_youtube_channel_id(youtube_username)};
	if (youtube_opt.has_value()) {
		const auto& youtube_id{youtube_opt.value()};

		NotificationRepository repo;
		const NotificationDTO data{guild_id, channel_id, "youtube", youtube_id, message, 500};

		if (repo.create(data)) {
			constexpr size_t timestep_sec{500};
			start_notification_deamon(channel_id, youtube_id, message, timestep_sec);
			Core::timed_reply_private(*Bot::ctx, event, "Notifications for youtube enabled!", 2000);
		} else {
			Core::timed_reply_private(*Bot::ctx, event, "Can't find the given youtube channel!", 2000);
		}
	} else {
		Core::timed_reply_private(*Bot::ctx, event, "Error: Failed to enable Upload notifications!", 2000);
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
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
