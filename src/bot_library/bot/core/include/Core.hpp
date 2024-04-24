/*
 *  (c) Copyright dropsoft.org - All rights reserved
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
	static auto is_admin(const dpp::guild_member& member) noexcept -> bool;

	static auto is_dropsoft_admin(const dpp::guild_member& member) noexcept -> bool;

	static auto get_role_id(const std::string& mention) noexcept -> std::string;

	static auto get_channel_id(const std::string& mention) noexcept -> std::string;

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

	[[nodiscard]] static auto get_parameter(dpp::cluster& bot,
											const dpp::slashcommand_t event,
											const std::string& name) noexcept -> std::string;

	[[nodiscard]] static auto simple_hash(const std::string& string) -> std::string;
};