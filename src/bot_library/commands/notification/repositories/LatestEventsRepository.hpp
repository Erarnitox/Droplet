#pragma once

#include <map>
#include <string>

class LatestEventsRepository {
  private:
	static std::map<std::string, std::string> latest_events;

  public:
	[[nodiscard]] static auto insert(const std::string& key, const std::string& value) -> bool;

	[[nodiscard]] static auto remove(const std::string& key) -> bool;

	[[nodiscard]] static auto exists(const std::string& key, const std::string& value) -> bool;

	[[nodiscard]] static auto load() -> bool;
};