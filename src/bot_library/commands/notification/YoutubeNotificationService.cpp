/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "YoutubeNotificationService.hpp"

#include <cpr/cpr.h>
#include <message.h>
#include <queues.h>

#include <Core.hpp>
#include <NotificationRepository.hpp>
#include <format>
#include <optional>
#include <regex>

#include "YoutubeFeed.hpp"

namespace {

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
	}
	return std::nullopt;
}

}  // namespace

std::optional<std::string> resolve_youtube_channel(const std::string& input) {
	return resolve_youtube_channel_id(input);
}

YoutubeNotificationService::YoutubeNotificationService(dpp::cluster& discord, DatabaseExecutor& db)
	: discord_(discord), db_(db), events_(db) {
}

void YoutubeNotificationService::start_job(std::size_t channel_id,
										   const std::string& youtube_id,
										   const std::string& message,
										   std::size_t timestep_sec) {
	const auto key{std::format("{}/{}", channel_id, youtube_id)};

	const dpp::timer_callback_t on_tick{[this, channel_id, youtube_id, message, key](dpp::timer timer_handle) {
		events_.set_active(key, true);
		const auto url{std::format("https://www.youtube.com/feeds/videos.xml?channel_id={}", youtube_id)};

		discord_.request(url,
						 dpp::m_get,
						 [this, channel_id, message, key, timer_handle](const dpp::http_request_completion_t& cc) {
							 const auto stop_timer_and_release = [this, key, timer_handle]() {
								 discord_.stop_timer(timer_handle);
								 std::lock_guard<std::mutex> lock(mutex_);
								 const auto it{timers_.find(key)};
								 if (it != timers_.end() && it->second == timer_handle) {
									 timers_.erase(it);
								 }
							 };

							 if (cc.status < 200 || cc.status >= 500) {
								 return;
							 }
							 if (cc.status >= 400) {
								 stop_timer_and_release();
								 return;
							 }
							 if (cc.status >= 300) {
								 return;
							 }

							 if (not events_.is_active(key)) {
								 stop_timer_and_release();
								 return;
							 }

							 const auto entries{parse_youtube_feed(cc.body)};
							 if (entries.empty()) {
								 return;
							 }

							 const auto known{events_.announced_ids(key)};
							 const auto plan{plan_youtube_uploads(entries, known)};

							 for (const auto& entry : plan.to_seed) {
								 (void)events_.seed_video(key, entry.video_id, entry.title);
							 }

							 for (const auto& entry : plan.to_announce) {
								 const auto claim{events_.try_claim_video(key, entry.video_id, entry.title)};
								 if (claim != YoutubeClaimResult::Claimed) {
									 continue;
								 }

								 const auto yt_link{std::format("https://www.youtube.com/watch?v={}", entry.video_id)};
								 dpp::message msg(
									 channel_id,
									 std::format("{}\n{}", Core::strip_broadcast_mentions(message), yt_link));
								 Core::disable_all_mentions(msg);
								 discord_.message_create(msg);
							 }
						 },
						 "",
						 "text/plain",
						 {{"User-Agent", "DropletDiscordBot (https://github.com/Erarnitox/DropletDiscordBot)"},
						  {"Accept", "application/atom+xml, application/xml;q=0.9, text/xml;q=0.8, */*;q=0.1"}});
	}};

	std::lock_guard<std::mutex> lock(mutex_);
	if (const auto it{timers_.find(key)}; it != timers_.end()) {
		discord_.stop_timer(it->second);
		timers_.erase(it);
	}
	const dpp::timer new_handle{discord_.start_timer(on_tick, timestep_sec)};
	timers_.emplace(key, new_handle);
}

void YoutubeNotificationService::stop_job(const std::string& key) {
	events_.set_active(key, false);
	(void)events_.remove(key);

	std::lock_guard<std::mutex> lock(mutex_);
	const auto it{timers_.find(key)};
	if (it == timers_.end()) {
		return;
	}
	discord_.stop_timer(it->second);
	timers_.erase(it);
}

void YoutubeNotificationService::load_from_database() {
	if (not events_.load()) {
		return;
	}
	NotificationRepository repo{db_};
	for (auto& job : repo.getAll()) {
		if (job.type != "youtube") {
			continue;
		}
		start_job(job.channel_id, job.data, job.message, job.timestep);
	}
}

void YoutubeNotificationService::stop_all() {
	std::lock_guard<std::mutex> lock(mutex_);
	for (auto& [key, timer] : timers_) {
		events_.set_active(key, false);
		discord_.stop_timer(timer);
	}
	timers_.clear();
}
