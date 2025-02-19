#pragma once

#include <map>
#include <string>

class LatestEventsRepository {
  private:
	static std::map<std::string, std::string> latest_events;
	static std::map<std::string, bool> active_events;
	
	static auto set_active(const std::string& key, bool active) -> void;
  public:
	[[nodiscard]] static auto insert(const std::string& key, const std::string& value) -> bool;

	[[nodiscard]] static auto remove(const std::string& key) -> bool;

	[[nodiscard]] static auto exists(const std::string& key, const std::string& value) -> bool;

	[[nodiscard]] static auto load() -> bool;

	[[nodiscard]] static auto is_active(const std::string& key) -> bool;
};