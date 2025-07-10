/*
 *  (c) Copyright erarnitox - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class offers access to all the
 * 	core functionality and utility needed by the bot
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/core
 */

#pragma once

#include <dpp/dpp.h>
#include <dpp/misc-enum.h>

#include <Database.hpp>
#include <IButtonCommand.hpp>
#include <IFormCommand.hpp>
#include <IGlobalSlashCommand.hpp>
#include <IMemberCommand.hpp>
#include <IMessageCommand.hpp>
#include <IReactionCommand.hpp>
#include <string>

class Core {
  public:
	static bool is_admin(const dpp::guild_member& member) noexcept;

	static bool is_erarnitox_admin(const dpp::guild_member& member) noexcept;

	static std::string get_role_id(const std::string& mention) noexcept;

	static std::string get_channel_id(const std::string& mention) noexcept;

	static void timed_reply(dpp::cluster& bot,
							const dpp::slashcommand_t event,
							const std::string& message,
							size_t time_mills) noexcept;

	static void timed_reply(dpp::cluster& bot,
							const dpp::form_submit_t event,
							const std::string& message,
							size_t time_mills) noexcept;

	static void timed_reply_private(dpp::cluster& bot,
									const dpp::slashcommand_t event,
									const std::string& message,
									size_t time_mills) noexcept;

	static void timed_reply_private(dpp::cluster& bot,
									const dpp::form_submit_t event,
									const std::string& message,
									size_t time_mills) noexcept;

	[[nodiscard]] static std::string get_parameter(dpp::cluster& bot,
												   const dpp::slashcommand_t event,
												   const std::string& name,
												   bool required = true) noexcept;

	[[nodiscard]] static std::string simple_hash(const std::string& string) noexcept;
};