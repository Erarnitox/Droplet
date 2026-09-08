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

#include "YoutubeFeed.hpp"

#include <algorithm>
#include <optional>
#include <regex>
#include <utility>

namespace {

[[nodiscard]] std::string decode_xml_text(const std::string& text) {
	std::string out;
	out.reserve(text.size());

	for (size_t i = 0; i < text.size();) {
		if (text[i] == '&') {
			if (text.compare(i, size_t{5}, "&amp;") == 0) {
				out.push_back('&');
				i += size_t{5};
			} else if (text.compare(i, size_t{4}, "&lt;") == 0) {
				out.push_back('<');
				i += size_t{4};
			} else if (text.compare(i, size_t{4}, "&gt;") == 0) {
				out.push_back('>');
				i += size_t{4};
			} else if (text.compare(i, size_t{6}, "&quot;") == 0) {
				out.push_back('"');
				i += size_t{6};
			} else if (text.compare(i, size_t{6}, "&apos;") == 0) {
				out.push_back('\'');
				i += size_t{6};
			} else {
				out.push_back(text[i]);
				++i;
			}
		} else {
			out.push_back(text[i]);
			++i;
		}
	}

	return out;
}

[[nodiscard]] bool published_newer_than(const std::string& candidate, const std::string& frontier) {
	if (candidate.empty()) {
		return false;
	}
	if (frontier.empty()) {
		return true;
	}
	return candidate > frontier;
}

}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<YoutubeFeedEntry> parse_youtube_feed(const std::string& atom_body) {
	static const std::regex entry_regex{R"(<entry>([\s\S]*?)</entry>)"};
	static const std::regex video_id_regex{R"(<yt:videoId>([^<]+)</yt:videoId>)"};
	static const std::regex title_regex{R"(<title[^>]*>([^<]*)</title>)"};
	static const std::regex published_regex{R"(<published>([^<]+)</published>)"};

	std::vector<YoutubeFeedEntry> entries;
	for (std::sregex_iterator it{atom_body.begin(), atom_body.end(), entry_regex}, end{}; it != end; ++it) {
		const std::string entry{(*it)[1].str()};

		std::smatch video_match;
		if (not std::regex_search(entry, video_match, video_id_regex) || video_match.size() < 2) {
			continue;
		}

		YoutubeFeedEntry parsed;
		parsed.video_id = video_match[1].str();

		std::smatch title_match;
		if (std::regex_search(entry, title_match, title_regex) && title_match.size() > 1) {
			parsed.title = decode_xml_text(title_match[1].str());
		}

		std::smatch published_match;
		if (std::regex_search(entry, published_match, published_regex) && published_match.size() > 1) {
			parsed.published = published_match[1].str();
		}

		entries.push_back(std::move(parsed));
	}

	return entries;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
YoutubeFeedPlan plan_youtube_uploads(const std::vector<YoutubeFeedEntry>& entries,
									 const std::unordered_set<std::string>& announced_ids) {
	YoutubeFeedPlan plan;
	if (entries.empty()) {
		return plan;
	}

	if (announced_ids.empty()) {
		plan.to_seed = entries;
		return plan;
	}

	std::optional<std::string> frontier;
	for (const auto& entry : entries) {
		if (not announced_ids.contains(entry.video_id) || entry.published.empty()) {
			continue;
		}
		if (not frontier.has_value() || entry.published > frontier.value()) {
			frontier = entry.published;
		}
	}

	if (not frontier.has_value()) {
		for (const auto& entry : entries) {
			if (not announced_ids.contains(entry.video_id)) {
				plan.to_seed.push_back(entry);
			}
		}
		return plan;
	}

	for (const auto& entry : entries) {
		if (announced_ids.contains(entry.video_id)) {
			continue;
		}
		if (published_newer_than(entry.published, frontier.value())) {
			plan.to_announce.push_back(entry);
		} else {
			plan.to_seed.push_back(entry);
		}
	}

	std::sort(plan.to_announce.begin(),
			  plan.to_announce.end(),
			  [](const YoutubeFeedEntry& lhs, const YoutubeFeedEntry& rhs) { return lhs.published < rhs.published; });

	return plan;
}
