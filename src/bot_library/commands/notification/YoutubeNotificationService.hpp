#pragma once

#include <dpp/cluster.h>
#include <dpp/timer.h>

#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "LatestEventsRepository.hpp"

class DatabaseExecutor;

[[nodiscard]] std::optional<std::string> resolve_youtube_channel(const std::string& input);

[[nodiscard]] std::string format_youtube_notification(std::string_view custom_message,
													  std::string_view video_url,
													  bool allow_broadcast);

class YoutubeNotificationService {
  public:
	YoutubeNotificationService(dpp::cluster& discord, DatabaseExecutor& db);

	void start_job(std::size_t channel_id,
				   const std::string& youtube_id,
				   const std::string& message,
				   std::size_t timestep_sec = 500,
				   bool allow_broadcast = false);
	void stop_job(const std::string& key);
	/// @return false if the database was not available (caller should retry later)
	[[nodiscard]] bool load_from_database();
	void stop_all();

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
	LatestEventsRepository events_;
	std::mutex mutex_;
	std::unordered_map<std::string, dpp::timer> timers_;
};
