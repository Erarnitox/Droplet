/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "LearnCppTutorialHint.hpp"

#include <message.h>
#include <snowflake.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

//-----------------------------------------------------
//
//-----------------------------------------------------
namespace {

//-----------------------------------------------------
//
//-----------------------------------------------------
constexpr std::string_view kPlaylistReply =
	"If you want to learn C++, there is this awesome free Playlist created by Erarnitox: "
	"https://www.youtube.com/watch?v=2O3r5qqjyiY&list=PLTjUlazALHSBQp4jdqHTCduTSSMU-cz5P";

//-----------------------------------------------------
//
//-----------------------------------------------------
constexpr auto kCooldown = std::chrono::minutes(10);

//-----------------------------------------------------
//
//-----------------------------------------------------
std::mutex g_cooldown_mutex;
std::unordered_map<std::string, std::chrono::steady_clock::time_point> g_last_reply_by_user_guild;

//-----------------------------------------------------
//
//-----------------------------------------------------
[[nodiscard]] std::string lowercase_ascii(std::string_view text) {
	std::string out(text.begin(), text.end());
	std::transform(
		out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return out;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
[[nodiscard]] bool mentions_cpp_learning_intent(std::string_view original_text) {
	const std::string lower = lowercase_ascii(original_text);

	static constexpr std::array<std::string_view, 28> phrases{
		"how to learn c++",
		"how do i learn c++",
		"how can i learn c++",
		"where to learn c++",
		"where can i learn c++",
		"want to learn c++",
		"wanna learn c++",
		"learn c++",
		"learning c++",
		"c++ tutorial",
		"tutorial for c++",
		"tutorials for c++",
		"cpp tutorial",
		"tutorial cpp",
		"learn cpp",
		"learning cpp",
		"how to learn cpp",
		"how do i learn cpp",
		"start learning c++",
		"getting started with c++",
		"beginner c++",
		"new to c++",
		"teach me c++",
		"best way to learn c++",
		"good resource c++",
		"resources to learn c++",
		"any tutorial c++",
		"recommend c++ course",
	};

	for (const std::string_view phrase : phrases) {
		if (lower.find(phrase) != std::string::npos) {
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
[[nodiscard]] bool cooldown_allows(uint64_t guild_id, uint64_t user_id) {
	const std::string key{std::format("{}:{}", guild_id, user_id)};
	const auto now{std::chrono::steady_clock::now()};

	std::lock_guard<std::mutex> lock(g_cooldown_mutex);
	const auto it{g_last_reply_by_user_guild.find(key)};
	if (it != g_last_reply_by_user_guild.end()) {
		if (now - it->second < kCooldown) {
			return false;
		}
	}
	g_last_reply_by_user_guild[key] = now;
	return true;
}

}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
LearnCppTutorialHint::LearnCppTutorialHint() = default;

//-----------------------------------------------------
//
//-----------------------------------------------------
void LearnCppTutorialHint::on_message_create(const dpp::message_create_t& event) {
	if (event.msg.author.is_bot()) {
		return;
	}

	if (event.msg.guild_id.empty()) {
		return;
	}

	if (event.msg.content.size() < 12) {
		return;
	}

	if (not mentions_cpp_learning_intent(event.msg.content)) {
		return;
	}

	if (not cooldown_allows(static_cast<uint64_t>(event.msg.guild_id), static_cast<uint64_t>(event.msg.author.id))) {
		return;
	}

	dpp::message reply(event.msg.channel_id, std::string(kPlaylistReply));
	reply.set_reference(event.msg.id, event.msg.guild_id, event.msg.channel_id, false);
	reply.set_allowed_mentions(false, false, false, false);

	Bot::ctx->message_create(reply);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void LearnCppTutorialHint::on_message_delete(const dpp::message_delete_t& event) {
	(void)event;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void LearnCppTutorialHint::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	(void)event;
}
