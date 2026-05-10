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

#include "LatestEventsRepository.hpp"

#include <DatabaseExecutor.hpp>
#include <mutex>

/*
CREATE TABLE public.latest_events
(
  id serial4 PRIMARY KEY,
  key varchar,
  latest varchar
);
*/

std::mutex LatestEventsRepository::active_events_mutex{};
std::mutex LatestEventsRepository::latest_events_mutex{};
std::unordered_map<std::string, bool> LatestEventsRepository::active_events{};
std::unordered_map<std::string, std::string> LatestEventsRepository::latest_events{};

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::is_active(const std::string& key) noexcept {
	const auto lock{std::lock_guard<std::mutex>(active_events_mutex)};
	if (not active_events.contains(key))
		return false;
	return active_events.at(key);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void LatestEventsRepository::set_active(const std::string& key, bool active) noexcept {
	const auto lock{std::lock_guard<std::mutex>(active_events_mutex)};
	active_events[key] = active;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::insert(const std::string& key, const std::string& value) noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	auto& exec = DatabaseExecutor::application_instance();
	if (not exec.hasConnection()) {
		return false;
	}

	if (latest_events.contains(key)) {
		latest_events[key] = value;

		static std::string sql_string("UPDATE latest_events SET latest = $2::varchar WHERE key = $1::varchar");
		return exec.execQuery(sql_string, key, value);
	} else {
		latest_events[key] = value;

		static const std::string sql_string{
			"INSERT INTO latest_events"
			"(key, latest) VALUES "
			"($1::varchar, $2::varchar)"};

		return exec.execQuery(sql_string, key, value);
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::remove(const std::string& key) noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	static const std::string sql_string{"DELETE FROM latest_events WHERE key = $1::varchar"};

	auto& exec = DatabaseExecutor::application_instance();
	if (not exec.hasConnection()) {
		return false;
	}

	latest_events.erase(key);
	const bool disabled{exec.execQuery(sql_string, key)};
	set_active(key, not disabled);
	return disabled;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::exists(const std::string& key, const std::string& value) noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	return latest_events.contains(key) && latest_events.at(key) == value;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::try_claim_new_latest(const std::string& key, const std::string& yt_link) noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	auto& exec = DatabaseExecutor::application_instance();
	if (not exec.hasConnection()) {
		return false;
	}

	const auto it = latest_events.find(key);
	if (it != latest_events.end() && it->second == yt_link) {
		return false;
	}

	const bool key_existed = (it != latest_events.end());
	latest_events[key] = yt_link;

	static std::string update_sql{"UPDATE latest_events SET latest = $2::varchar WHERE key = $1::varchar"};
	static const std::string insert_sql{
		"INSERT INTO latest_events"
		"(key, latest) VALUES "
		"($1::varchar, $2::varchar)"};

	if (key_existed) {
		return exec.execQuery(update_sql, key, yt_link);
	}
	return exec.execQuery(insert_sql, key, yt_link);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::load() noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	static const std::string sql_string{"SELECT key, latest FROM latest_events"};
	const auto result{DatabaseExecutor::application_instance().execSelectAll(sql_string)};

	for (const auto& adapter : result) {
		latest_events.insert_or_assign(adapter.get<std::string>("key"), adapter.get<std::string>("latest"));
	}

	return true;
}