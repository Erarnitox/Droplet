#include "LatestEventsRepository.hpp"

#include <Database.hpp>
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

auto LatestEventsRepository::is_active(const std::string& key) -> bool {
	const auto lock{std::lock_guard<std::mutex>(active_events_mutex)};
	if (not active_events.contains(key))
		return false;
	return active_events.at(key);
}

auto LatestEventsRepository::set_active(const std::string& key, bool active) -> void {
	const auto lock{std::lock_guard<std::mutex>(active_events_mutex)};
	active_events[key] = active;
}

auto LatestEventsRepository::insert(const std::string& key, const std::string& value) -> bool {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	if (!Database::hasConnection()) {
		return false;
	}

	if (latest_events.contains(key)) {
		latest_events[key] = value;

		static std::string sql_string("UPDATE latest_events SET latest = $2::varchar WHERE key = $1::varchar");
		return database::execQuery(sql_string, key, value);
	} else {
		latest_events[key] = value;

		static const std::string sql_string{
			"INSERT INTO latest_events"
			"(key, latest) VALUES "
			"($1::varchar, $2::varchar)"};

		return database::execQuery(sql_string, key, value);
	}
}

auto LatestEventsRepository::remove(const std::string& key) -> bool {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	static const std::string sql_string{"DELETE FROM latest_events WHERE key = $1::varchar"};

	if (!Database::hasConnection()) {
		return false;
	}

	latest_events.erase(key);
	const bool disabled{database::execQuery(sql_string, key)};
	set_active(key, not disabled);
	return disabled;
}

auto LatestEventsRepository::exists(const std::string& key, const std::string& value) -> bool {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	return latest_events.contains(key) && latest_events.at(key) == value;
}

auto LatestEventsRepository::load() -> bool {
	const auto lock{std::lock_guard<std::mutex>(latest_events_mutex)};
	static const std::string sql_string{"SELECT key, latest FROM latest_events"};
	auto result{database::execSelectAll(sql_string)};

	for (const auto& adapter : result) {
		latest_events.insert_or_assign(adapter.get<std::string>("key"), adapter.get<std::string>("latest"));
	}

	return true;
}