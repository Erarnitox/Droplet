#include "LatestEventsRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <mutex>
#include <vector>

/*
CREATE TABLE public.latest_events
(
  id serial4 NOT NULL,
  key varchar,
  latest varchar
);
*/

static std::mutex active_map_mutex;
std::map<std::string, bool> LatestEventsRepository::active_events{};
std::map<std::string, std::string> LatestEventsRepository::latest_events{};

auto LatestEventsRepository::is_active(const std::string& key) -> bool {
	if(not active_events.contains(key)) return false;
	return active_events.at(key);
}

auto LatestEventsRepository::set_active (const std::string& key, bool active) -> void {
	std::lock_guard<std::mutex> lock(active_map_mutex);
	active_events[key] = active;
}

auto LatestEventsRepository::insert(const std::string& key, const std::string& value) -> bool {
	if (!Database::hasConnection()) {
		return false;
	}

	if (latest_events.contains(key)) {
		latest_events[key] = value;

		static std::string sql_string("UPDATE latest_events SET latest = $2::varchar WHERE key = $1::varchar");
		return database::execQuery(sql_string, key, value);
	} else {
		latest_events[key] = value;
		
		static std::string sql_string{
			"INSERT INTO latest_events"
			"(key, latest) VALUES "
			"($1::varchar, $2::varchar)"};
		const bool active{ database::execQuery(sql_string, key, value) };
		set_active(key, active);
		return active;
	}
}

auto LatestEventsRepository::remove(const std::string& key) -> bool {
	static std::string sql_string{"DELETE FROM latest_events WHERE key = $1::varchar"};

	if (!Database::hasConnection()) {
		return false;
	}

	latest_events.erase(key);
	const bool disabled{ database::execQuery(sql_string, key) };
	set_active(key, not disabled);
	return disabled;
}

auto LatestEventsRepository::exists(const std::string& key, const std::string& value) -> bool {
	return latest_events.contains(key) && latest_events.at(key) == value;
}

auto LatestEventsRepository::load() -> bool {
	static std::string sql_string{"SELECT key, latest FROM latest_events"};
	auto result{database::execSelectAll(sql_string)};

	for (const auto& adapter : result) {
		latest_events.insert_or_assign(adapter.get<std::string>("key"), adapter.get<std::string>("latest"));
		active_events.insert_or_assign(adapter.get<std::string>("key"), true);
	}

	active_map_mutex.unlock();
	return true;
}