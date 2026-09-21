/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "LearnCppTutorialHint.hpp"

#include <message.h>
#include <snowflake.h>

#include <AppContext.hpp>
#include <chrono>
#include <cstdint>
#include <functional>
#include <mutex>
#include <unordered_map>

#include "CppLearningIntent.hpp"

namespace {

constexpr std::string_view kPlaylistReply =
	"If you want to learn C++, there is this awesome free Playlist created by Erarnitox: "
	"https://www.youtube.com/watch?v=2O3r5qqjyiY&list=PLTjUlazALHSBQp4jdqHTCduTSSMU-cz5P";

constexpr auto kCooldown = std::chrono::minutes(10);

struct CooldownKey {
	std::uint64_t guild_id;
	std::uint64_t user_id;
	bool operator==(const CooldownKey&) const = default;
};

struct CooldownKeyHash {
	std::size_t operator()(const CooldownKey& key) const noexcept {
		return std::hash<std::uint64_t>{}(key.guild_id) ^ (std::hash<std::uint64_t>{}(key.user_id) << 1);
	}
};

std::mutex g_cooldown_mutex;
std::unordered_map<CooldownKey, std::chrono::steady_clock::time_point, CooldownKeyHash> g_last_reply_by_user_guild;

[[nodiscard]] bool cooldown_allows(uint64_t guild_id, uint64_t user_id) {
	const CooldownKey key{.guild_id = guild_id, .user_id = user_id};
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

LearnCppTutorialHint::LearnCppTutorialHint(AppContext& ctx) : discord_(ctx.discord) {
}

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

	if (not cpp_learning_intent::mentions_cpp_learning_intent(event.msg.content)) {
		return;
	}

	if (not cooldown_allows(static_cast<uint64_t>(event.msg.guild_id), static_cast<uint64_t>(event.msg.author.id))) {
		return;
	}

	dpp::message reply(event.msg.channel_id, std::string(kPlaylistReply));
	reply.set_reference(event.msg.id, event.msg.guild_id, event.msg.channel_id, false);
	reply.set_allowed_mentions(false, false, false, false);

	discord_.message_create(reply);
}
