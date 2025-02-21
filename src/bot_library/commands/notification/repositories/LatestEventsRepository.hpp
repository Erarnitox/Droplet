#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

class LatestEventsRepository {
  private:
	static std::unordered_map<std::string, std::string> latest_events;
	static std::unordered_map<std::string, bool> active_events;
	static std::mutex active_events_mutex;
	static std::mutex latest_events_mutex;

  public:
	[[nodiscard]] static auto insert(const std::string& key, const std::string& value) -> bool;

	[[nodiscard]] static auto remove(const std::string& key) -> bool;

	[[nodiscard]] static auto exists(const std::string& key, const std::string& value) -> bool;

	[[nodiscard]] static auto load() -> bool;

	static auto set_active(const std::string& key, bool active) -> void;

	[[nodiscard]] static auto is_active(const std::string& key) -> bool;
};