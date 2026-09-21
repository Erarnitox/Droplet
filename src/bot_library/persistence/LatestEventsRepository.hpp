/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

class DatabaseExecutor;

enum class YoutubeClaimResult {
	AlreadyAnnounced,
	Seeded,
	Claimed,
	Failed,
};

class LatestEventsRepository {
  public:
	explicit LatestEventsRepository(DatabaseExecutor& executor);

	[[nodiscard]] bool insert(const std::string& key, const std::string& value) noexcept;
	[[nodiscard]] bool remove(const std::string& key) noexcept;
	[[nodiscard]] bool exists(const std::string& key, const std::string& value) noexcept;
	[[nodiscard]] YoutubeClaimResult try_claim_video(const std::string& key,
													 const std::string& video_id,
													 const std::string& title) noexcept;
	[[nodiscard]] YoutubeClaimResult seed_video(const std::string& key,
												const std::string& video_id,
												const std::string& title) noexcept;
	[[nodiscard]] bool has_announced(const std::string& key) noexcept;
	[[nodiscard]] std::unordered_set<std::string> announced_ids(const std::string& key) noexcept;
	[[nodiscard]] bool load() noexcept;
	void set_active(const std::string& key, bool active) noexcept;
	[[nodiscard]] bool is_active(const std::string& key) noexcept;

  private:
	[[nodiscard]] YoutubeClaimResult record_video(const std::string& key,
												  const std::string& video_id,
												  const std::string& title,
												  bool allow_announce) noexcept;

	DatabaseExecutor& executor_;
	std::unordered_map<std::string, std::string> latest_events;
	std::unordered_map<std::string, std::unordered_set<std::string>> announced_videos;
	std::unordered_map<std::string, bool> active_events;
	std::mutex active_events_mutex;
	std::mutex latest_events_mutex;
};
