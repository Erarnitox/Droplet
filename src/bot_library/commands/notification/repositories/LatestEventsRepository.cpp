#include "LatestEventsRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

/*
CREATE TABLE public.latest_events
(
  id serial4 NOT NULL,
  key varchar,
  latest varchar
);
*/

std::map<std::string, std::string> LatestEventsRepository::latest_events{};

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

		return database::execQuery(sql_string, key, value);
	}
}

auto LatestEventsRepository::remove(const std::string& key) -> bool {
	static std::string sql_string{"DELETE FROM latest_events WHERE key = $1::varchar"};

	if (!Database::hasConnection()) {
		return false;
	}

	latest_events.erase(key);

	return database::execQuery(sql_string, key);
}

auto LatestEventsRepository::exists(const std::string& key, const std::string& value) -> bool {
	return latest_events.contains(key) && latest_events.at(key) == value;
}

auto LatestEventsRepository::load() -> bool {
	static std::string sql_string{"SELECT key, latest FROM latest_events"};
	auto result{database::execSelectAll(sql_string)};

	for (const auto& adapter : result) {
		latest_events.insert_or_assign(adapter.get<std::string>("key"), adapter.get<std::string>("latest"));
	}

	return true;
}