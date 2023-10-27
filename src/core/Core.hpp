#pragma once

#include <dpp/dpp.h>
#include <dpp/misc-enum.h>
#include <fmt/core.h>

#include "database/Database.hpp"
#include "interfaces/IButtonCommand.hpp"
#include "interfaces/IFormCommand.hpp"
#include "interfaces/IGlobalSlashCommand.hpp"
#include "interfaces/IMemberCommand.hpp"
#include "interfaces/IMessageCommand.hpp"
#include "interfaces/IReactionCommand.hpp"

class Core {
  public:
	static auto isAdmin(const dpp::guild_member &member) noexcept -> bool;

	static auto getRoleId(const std::string &mention) noexcept -> std::string;

	static auto getChannelId(const std::string &mention) noexcept -> std::string;

	static auto timedReply(dpp::cluster &bot,
						   const dpp::slashcommand_t event,
						   const std::string &message,
						   size_t time_mills) noexcept -> void;

	static auto timedReply(dpp::cluster &bot,
						   const dpp::form_submit_t event,
						   const std::string &message,
						   size_t time_mills) noexcept -> void;

	static auto privateTimedReply(dpp::cluster &bot,
								  const dpp::slashcommand_t event,
								  const std::string &message,
								  size_t time_mills) noexcept -> void;

	static auto privateTimedReply(dpp::cluster &bot,
								  const dpp::form_submit_t event,
								  const std::string &message,
								  size_t time_mills) noexcept -> void;

	[[nodiscard]] static auto getParameter(dpp::cluster &bot,
										   const dpp::slashcommand_t event,
										   const std::string &name) noexcept -> std::string;
};