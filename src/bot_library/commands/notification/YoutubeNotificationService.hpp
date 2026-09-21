#pragma once

#include <dpp/cluster.h>
#include <dpp/timer.h>

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "LatestEventsRepository.hpp"

class DatabaseExecutor;

[[nodiscard]] std::optional<std::string> resolve_youtube_channel(const std::string& input);

class YoutubeNotificationService {
  public:
	YoutubeNotificationService(dpp::cluster& discord, DatabaseExecutor& db);

	void start_job(std::size_t channel_id,
				   const std::string& youtube_id,
				   const std::string& message,
				   std::size_t timestep_sec = 500);
	void stop_job(const std::string& key);
	void load_from_database();
	void stop_all();

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
	LatestEventsRepository events_;
	std::mutex mutex_;
	std::unordered_map<std::string, dpp::timer> timers_;
};
