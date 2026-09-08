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

#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

//-----------------------------------------------------
//
//-----------------------------------------------------
enum class YoutubeClaimResult {
	AlreadyAnnounced,  // video id already recorded; do not post
	Seeded,	 // first observation for this key; record without posting
	Claimed,  // new video id claimed; caller should post
	Failed,	 // DB/connection failure; do not post
};

//-----------------------------------------------------
//
//-----------------------------------------------------
class LatestEventsRepository {
  private:
	static std::unordered_map<std::string, std::string> latest_events;
	static std::unordered_map<std::string, std::unordered_set<std::string>> announced_videos;
	static std::unordered_map<std::string, bool> active_events;
	static std::mutex active_events_mutex;
	static std::mutex latest_events_mutex;

	[[nodiscard]] static YoutubeClaimResult record_video(const std::string& key,
														 const std::string& video_id,
														 const std::string& title,
														 bool allow_announce) noexcept;

  public:
	[[nodiscard]] static bool insert(const std::string& key, const std::string& value) noexcept;

	[[nodiscard]] static bool remove(const std::string& key) noexcept;

	[[nodiscard]] static bool exists(const std::string& key, const std::string& value) noexcept;

	[[nodiscard]] static YoutubeClaimResult try_claim_video(const std::string& key,
															const std::string& video_id,
															const std::string& title) noexcept;

	/** Record \p video_id without announcing. Never returns Claimed. */
	[[nodiscard]] static YoutubeClaimResult seed_video(const std::string& key,
													   const std::string& video_id,
													   const std::string& title) noexcept;

	[[nodiscard]] static bool has_announced(const std::string& key) noexcept;

	[[nodiscard]] static std::unordered_set<std::string> announced_ids(const std::string& key) noexcept;

	[[nodiscard]] static bool load() noexcept;

	static void set_active(const std::string& key, bool active) noexcept;

	[[nodiscard]] static bool is_active(const std::string& key) noexcept;
};
