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
#include <algorithm>
#include <exception>
#include <format>
#include <optional>
#include <string_view>

#include "YoutubeFeed.hpp"

namespace {

[[nodiscard]] bool is_id_char(char c) noexcept {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
}

// a YouTube channel id: "UC" followed by 22 id characters (lenient on the length)
[[nodiscard]] bool looks_like_channel_id(std::string_view text) noexcept {
	return text.size() >= 12 && text.size() <= 40 && text.starts_with("UC") && std::ranges::all_of(text, is_id_char);
}

// a channel handle / legacy user name as it can appear in youtube.com/@handle
[[nodiscard]] bool looks_like_handle(std::string_view text) noexcept {
	return not text.empty() && text.size() <= 100 &&
		std::ranges::all_of(text, [](char c) { return is_id_char(c) || c == '.'; });
}

// returns the run of [a-zA-Z0-9_-] that directly follows the first usable `marker` in `text`
[[nodiscard]] std::optional<std::string> capture_after(std::string_view text, std::string_view marker) {
	for (auto pos{text.find(marker)}; pos != std::string_view::npos; pos = text.find(marker, pos)) {
		const auto start{pos + marker.size()};
		auto end{start};
		while (end < text.size() && is_id_char(text[end])) {
			++end;
		}
		if (end > start) {
			return std::string{text.substr(start, end - start)};
		}
		pos = start;
	}
	return std::nullopt;
}

[[nodiscard]] std::optional<std::string> resolve_youtube_channel_id(const std::string& input) {
	if (looks_like_channel_id(input)) {
		return input;
	}

	if (const auto channel{capture_after(input, "youtube.com/channel/")}) {
		return channel;
	}

	std::string username;
	if (const auto user{capture_after(input, "youtube.com/user/")}) {
		username = *user;
	} else if (const auto handle{capture_after(input, "youtube.com/@")}) {
		username = *handle;
	} else {
		username = input;
		if (username.starts_with('@')) {
			username.erase(0, 1);
		}
	}

	// the name ends up in a URL: refuse anything that is not a plain handle
	if (not looks_like_handle(username)) {
		return std::nullopt;
	}

	// bounded in time: this runs on a worker thread of the Discord library
	const cpr::Response r{
		cpr::Get(cpr::Url{"https://youtube.com/@" + username}, cpr::ConnectTimeout{5000}, cpr::Timeout{10000})};
	if (r.status_code != 200) {
		return std::nullopt;
	}
	if (const auto channel{capture_after(r.text, "youtube.com/channel/")}) {
		return channel;
	}
	return std::nullopt;
}

}  // namespace

std::optional<std::string> resolve_youtube_channel(const std::string& input) {
	try {
		return resolve_youtube_channel_id(input);
	} catch (...) {
		return std::nullopt;  // network / parsing failure: report "not found" instead of crashing
	}
}

std::string format_youtube_notification(std::string_view custom_message,
										std::string_view video_url,
										bool allow_broadcast) {
	const std::string body{allow_broadcast ? std::string{custom_message}
										   : Core::strip_broadcast_mentions(custom_message)};
	return std::format("{}\n{}", body, video_url);
}

YoutubeNotificationService::YoutubeNotificationService(dpp::cluster& discord, DatabaseExecutor& db)
	: discord_(discord), db_(db), events_(db) {
}

void YoutubeNotificationService::start_job(std::size_t channel_id,
										   const std::string& youtube_id,
										   const std::string& message,
										   std::size_t timestep_sec,
										   bool allow_broadcast) {
	const auto key{std::format("{}/{}", channel_id, youtube_id)};

	const dpp::timer_callback_t on_tick{
		[this, channel_id, youtube_id, message, key, allow_broadcast](dpp::timer timer_handle) {
			// Timer callbacks run on the Discord event-loop thread. An exception escaping from here
			// would silently kill this timer (it is popped before the callback runs), so never throw.
			try {
				{
					std::lock_guard<std::mutex> lock(mutex_);
					const auto it{timers_.find(key)};
					if (it == timers_.end() || it->second != timer_handle) {
						return;
					}
					events_.set_active(key, true);
				}
				const auto url{std::format("https://www.youtube.com/feeds/videos.xml?channel_id={}", youtube_id)};

				discord_.request(
					url,
					dpp::m_get,
					[this, channel_id, message, key, timer_handle, allow_broadcast](
						const dpp::http_request_completion_t& cc) {
						try {
							const auto stop_timer_and_release = [this, key, timer_handle]() {
								std::lock_guard<std::mutex> lock(mutex_);
								const auto it{timers_.find(key)};
								if (it == timers_.end() || it->second != timer_handle) {
									return;
								}
								events_.set_active(key, false);
								discord_.stop_timer(it->second);
								timers_.erase(it);
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
								if (claim != YoutubeClaimResult::Claimed || not events_.is_active(key)) {
									continue;
								}

								const auto yt_link{std::format("https://www.youtube.com/watch?v={}", entry.video_id)};
								dpp::message msg(channel_id,
												 format_youtube_notification(message, yt_link, allow_broadcast));
								if (allow_broadcast) {
									msg.set_allowed_mentions(false, false, true, false);
								} else {
									Core::disable_all_mentions(msg);
								}
								discord_.message_create(msg);
							}
						} catch (const std::exception& ex) {
							discord_.log(dpp::ll_error,
										 std::format("YouTube feed handling for {} failed: {}", key, ex.what()));
						} catch (...) {
							discord_.log(dpp::ll_error, std::format("YouTube feed handling for {} failed", key));
						}
					},
					"",
					"text/plain",
					{{"User-Agent", "DropletDiscordBot (https://github.com/Erarnitox/DropletDiscordBot)"},
					 {"Accept", "application/atom+xml, application/xml;q=0.9, text/xml;q=0.8, */*;q=0.1"}});
			} catch (const std::exception& ex) {
				discord_.log(dpp::ll_error, std::format("YouTube poll for {} failed: {}", key, ex.what()));
			} catch (...) {
				discord_.log(dpp::ll_error, std::format("YouTube poll for {} failed with an unknown error", key));
			}
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
	{
		std::lock_guard<std::mutex> lock(mutex_);
		const auto it{timers_.find(key)};
		if (it != timers_.end()) {
			discord_.stop_timer(it->second);
			timers_.erase(it);
		}
	}
	// database work happens outside mutex_ so the timer callbacks (event-loop thread) never wait for it
	(void)events_.remove(key);
}

bool YoutubeNotificationService::load_from_database() {
	if (not events_.load()) {
		return false;
	}
	NotificationRepository repo{db_};
	for (auto& job : repo.getAll()) {
		if (job.type != "youtube") {
			continue;
		}
		start_job(job.channel_id, job.data, job.message, job.timestep, job.allow_broadcast);
	}
	return true;
}

void YoutubeNotificationService::stop_all() {
	std::lock_guard<std::mutex> lock(mutex_);
	for (auto& [key, timer] : timers_) {
		events_.set_active(key, false);
		discord_.stop_timer(timer);
	}
	timers_.clear();
}
