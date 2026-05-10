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

//-----------------------------------------------------
//
//-----------------------------------------------------
class LatestEventsRepository {
  private:
	static std::unordered_map<std::string, std::string> latest_events;
	static std::unordered_map<std::string, bool> active_events;
	static std::mutex active_events_mutex;
	static std::mutex latest_events_mutex;

  public:
	[[nodiscard]] static bool insert(const std::string& key, const std::string& value) noexcept;

	[[nodiscard]] static bool remove(const std::string& key) noexcept;

	[[nodiscard]] static bool exists(const std::string& key, const std::string& value) noexcept;

	/**
	 * If \p yt_link is already the stored latest for \p key, returns false.
	 * Otherwise updates memory + DB and returns true so the caller may announce once.
	 * Thread-safe vs other LatestEventsRepository calls; use this instead of exists()+insert().
	 */
	[[nodiscard]] static bool try_claim_new_latest(const std::string& key, const std::string& yt_link) noexcept;

	[[nodiscard]] static bool load() noexcept;

	static void set_active(const std::string& key, bool active) noexcept;

	[[nodiscard]] static bool is_active(const std::string& key) noexcept;
};