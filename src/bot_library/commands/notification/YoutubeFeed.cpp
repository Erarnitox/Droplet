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
#include <string_view>
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

// Finds the first `<open ...>text</close>` in `source` where text contains no '<'.
// With `allow_attributes` the opening tag may carry attributes (`open` is then "<title").
[[nodiscard]] std::optional<std::string> element_text(std::string_view source,
													  std::string_view open,
													  std::string_view close,
													  bool allow_attributes,
													  bool allow_empty) {
	for (auto pos{source.find(open)}; pos != std::string_view::npos; pos = source.find(open, pos + 1)) {
		auto text_begin{pos + open.size()};
		if (allow_attributes) {
			const auto tag_end{source.find('>', text_begin)};
			if (tag_end == std::string_view::npos) {
				return std::nullopt;
			}
			if (source.substr(text_begin, tag_end - text_begin).find('<') != std::string_view::npos) {
				continue;
			}
			text_begin = tag_end + 1;
		}
		const auto text_end{source.find('<', text_begin)};
		if (text_end == std::string_view::npos) {
			return std::nullopt;
		}
		if (source.compare(text_end, close.size(), close) == 0 && (allow_empty || text_end > text_begin)) {
			return std::string{source.substr(text_begin, text_end - text_begin)};
		}
	}
	return std::nullopt;
}

}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
std::vector<YoutubeFeedEntry> parse_youtube_feed(const std::string& atom_body) {
	// Plain string scanning instead of std::regex: libstdc++'s regex matcher recurses per
	// input character and can overflow the stack on large or hostile bodies.
	constexpr std::string_view entry_open{"<entry>"};
	constexpr std::string_view entry_close{"</entry>"};

	std::vector<YoutubeFeedEntry> entries;
	const std::string_view body{atom_body};

	for (auto open{body.find(entry_open)}; open != std::string_view::npos;) {
		const auto content_begin{open + entry_open.size()};
		const auto close{body.find(entry_close, content_begin)};
		if (close == std::string_view::npos) {
			break;
		}
		const auto entry{body.substr(content_begin, close - content_begin)};
		open = body.find(entry_open, close + entry_close.size());

		const auto video_id{element_text(entry, "<yt:videoId>", "</yt:videoId>", false, false)};
		if (not video_id.has_value()) {
			continue;
		}

		YoutubeFeedEntry parsed;
		parsed.video_id = *video_id;
		if (const auto title{element_text(entry, "<title", "</title>", true, true)}) {
			parsed.title = decode_xml_text(*title);
		}
		if (const auto published{element_text(entry, "<published>", "</published>", false, false)}) {
			parsed.published = *published;
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
