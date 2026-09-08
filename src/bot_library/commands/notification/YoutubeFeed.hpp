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

#pragma once

#include <string>
#include <unordered_set>
#include <vector>

struct YoutubeFeedEntry {
	std::string video_id;
	std::string title;
	std::string published;
};

struct YoutubeFeedPlan {
	std::vector<YoutubeFeedEntry> to_seed;
	std::vector<YoutubeFeedEntry> to_announce;
};

[[nodiscard]] std::vector<YoutubeFeedEntry> parse_youtube_feed(const std::string& atom_body);

[[nodiscard]] YoutubeFeedPlan plan_youtube_uploads(const std::vector<YoutubeFeedEntry>& entries,
												   const std::unordered_set<std::string>& announced_ids);
