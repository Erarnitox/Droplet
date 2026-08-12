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
#include <optional>
#include <regex>

/*
CREATE TABLE public.latest_events
(
  id serial4 PRIMARY KEY,
  key varchar,
  latest varchar
);

CREATE TABLE IF NOT EXISTS public.youtube_announced (
  key varchar NOT NULL,
  video_id varchar NOT NULL,
  title varchar NULL,
  PRIMARY KEY (key, video_id)
);
*/

namespace {

[[nodiscard]] std::optional<std::string> extract_video_id(const std::string& value) noexcept {
	if (value.empty()) {
		return std::nullopt;
	}

	static const std::regex watch_regex{R"(youtube\.com\/watch\?v=([a-zA-Z0-9_-]+))"};
	static const std::regex v_regex{R"(youtube\.com\/v\/([a-zA-Z0-9_-]+))"};
	static const std::regex bare_id_regex{R"(^[a-zA-Z0-9_-]{11}$)"};

	std::smatch match;
	if (std::regex_search(value, match, watch_regex) && match.size() > 1) {
		return match[1].str();
	}
	if (std::regex_search(value, match, v_regex) && match.size() > 1) {
		return match[1].str();
	}
	if (std::regex_match(value, match, bare_id_regex)) {
		return match[0].str();
	}
	return std::nullopt;
}

[[nodiscard]] bool persist_latest_tip(DatabaseExecutor& exec,
									  const std::string& key,
									  const std::string& video_id,
									  bool key_existed_in_latest) noexcept {
	static const std::string update_sql{"UPDATE latest_events SET latest = $2::varchar WHERE key = $1::varchar"};
	static const std::string insert_sql{
		"INSERT INTO latest_events"
		"(key, latest) VALUES "
		"($1::varchar, $2::varchar)"};

	if (key_existed_in_latest) {
		return exec.execQuery(update_sql, key, video_id);
	}
	return exec.execQuery(insert_sql, key, video_id);
}

}  // namespace

std::mutex LatestEventsRepository::active_events_mutex{};
std::mutex LatestEventsRepository::latest_events_mutex{};
std::unordered_map<std::string, bool> LatestEventsRepository::active_events{};
std::unordered_map<std::string, std::string> LatestEventsRepository::latest_events{};
std::unordered_map<std::string, std::unordered_set<std::string>> LatestEventsRepository::announced_videos{};

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
	static const std::string delete_latest_sql{"DELETE FROM latest_events WHERE key = $1::varchar"};
	static const std::string delete_announced_sql{"DELETE FROM youtube_announced WHERE key = $1::varchar"};

	auto& exec = DatabaseExecutor::application_instance();
	if (not exec.hasConnection()) {
		return false;
	}

	latest_events.erase(key);
	announced_videos.erase(key);

	const bool deleted_latest{exec.execQuery(delete_latest_sql, key)};
	const bool deleted_announced{exec.execQuery(delete_announced_sql, key)};
	const bool disabled{deleted_latest || deleted_announced};
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
YoutubeClaimResult LatestEventsRepository::try_claim_video(const std::string& key,
														   const std::string& video_id,
														   const std::string& title) noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	auto& exec = DatabaseExecutor::application_instance();
	if (not exec.hasConnection() || video_id.empty()) {
		return YoutubeClaimResult::Failed;
	}

	auto& known = announced_videos[key];
	if (known.contains(video_id)) {
		return YoutubeClaimResult::AlreadyAnnounced;
	}

	const bool key_had_history = not known.empty();

	static const std::string insert_announced_sql{
		"INSERT INTO youtube_announced (key, video_id, title) VALUES "
		"($1::varchar, $2::varchar, $3::varchar) "
		"ON CONFLICT (key, video_id) DO NOTHING "
		"RETURNING video_id"};

	const auto inserted{exec.execSelectAll(insert_announced_sql, key, video_id, title)};
	if (inserted.empty()) {
		known.insert(video_id);
		return YoutubeClaimResult::AlreadyAnnounced;
	}

	known.insert(video_id);

	const bool tip_existed = latest_events.contains(key);
	latest_events[key] = video_id;
	if (not persist_latest_tip(exec, key, video_id, tip_existed)) {
		return YoutubeClaimResult::Failed;
	}

	return key_had_history ? YoutubeClaimResult::Claimed : YoutubeClaimResult::Seeded;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
bool LatestEventsRepository::load() noexcept {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	auto& exec = DatabaseExecutor::application_instance();
	if (not exec.hasConnection()) {
		return false;
	}

	static const std::string ensure_announced_sql{
		"CREATE TABLE IF NOT EXISTS public.youtube_announced ("
		"key varchar NOT NULL, "
		"video_id varchar NOT NULL, "
		"title varchar NULL, "
		"PRIMARY KEY (key, video_id)"
		")"};
	if (not exec.execQuery(ensure_announced_sql)) {
		return false;
	}

	announced_videos.clear();
	latest_events.clear();

	static const std::string select_announced_sql{"SELECT key, video_id FROM youtube_announced"};
	for (const auto& adapter : exec.execSelectAll(select_announced_sql)) {
		const auto key{adapter.get<std::string>("key")};
		const auto video_id{adapter.get<std::string>("video_id")};
		if (not key.empty() && not video_id.empty()) {
			announced_videos[key].insert(video_id);
		}
	}

	static const std::string select_latest_sql{"SELECT key, latest FROM latest_events"};
	for (const auto& adapter : exec.execSelectAll(select_latest_sql)) {
		const auto key{adapter.get<std::string>("key")};
		const auto latest{adapter.get<std::string>("latest")};
		if (key.empty()) {
			continue;
		}
		latest_events.insert_or_assign(key, latest);

		const auto video_id_opt{extract_video_id(latest)};
		if (not video_id_opt.has_value()) {
			continue;
		}
		const auto& video_id = video_id_opt.value();
		if (announced_videos[key].contains(video_id)) {
			continue;
		}

		static const std::string migrate_sql{
			"INSERT INTO youtube_announced (key, video_id, title) VALUES "
			"($1::varchar, $2::varchar, NULL) "
			"ON CONFLICT (key, video_id) DO NOTHING"};
		if (exec.execQuery(migrate_sql, key, video_id)) {
			announced_videos[key].insert(video_id);
		}
	}

	return true;
}
