/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: Discord helper utilities used by commands. Intentionally does
 *  not include Database or command interfaces (stable, minimal core surface).
 */

#pragma once

#include <dpp/dpp.h>
#include <dpp/misc-enum.h>

#include <string>
#include <string_view>

class Core {
  public:
	static bool is_admin(const dpp::guild_member& member) noexcept;

	static bool is_erarnitox_admin(const dpp::guild_member& member) noexcept;

	static std::string get_role_id(const std::string& mention) noexcept;

	static std::string get_channel_id(const std::string& mention) noexcept;

	static void timed_reply(dpp::cluster& bot,
							const dpp::slashcommand_t& event,
							const std::string& message,
							size_t time_mills) noexcept;

	static void timed_reply(dpp::cluster& bot,
							const dpp::form_submit_t& event,
							const std::string& message,
							size_t time_mills) noexcept;

	static void timed_reply_private(dpp::cluster& bot,
									const dpp::slashcommand_t& event,
									const std::string& message,
									size_t time_mills) noexcept;

	static void timed_reply_private(dpp::cluster& bot,
									const dpp::form_submit_t& event,
									const std::string& message,
									size_t time_mills) noexcept;

	[[nodiscard]] static std::string get_parameter(dpp::cluster& bot,
												   const dpp::slashcommand_t& event,
												   const std::string& name,
												   bool required = true) noexcept;

	[[nodiscard]] static std::string simple_hash(std::string_view string) noexcept;

	[[nodiscard]] static std::string strip_broadcast_mentions(std::string_view text);

	static void disable_all_mentions(dpp::message& message) noexcept;
};
