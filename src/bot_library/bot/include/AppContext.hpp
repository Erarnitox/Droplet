#pragma once

#include <dpp/cluster.h>

#include "CommandTypes.hpp"

class BlacklistIndex;
class DatabaseExecutor;
class PortalIndex;
class YoutubeNotificationService;

struct AppContext {
	dpp::cluster& discord;
	DatabaseExecutor& db;
	PortalIndex& portal_index;
	BlacklistIndex& blacklist_index;
	YoutubeNotificationService& youtube;
	slash_commands_t& slash_commands;
};
