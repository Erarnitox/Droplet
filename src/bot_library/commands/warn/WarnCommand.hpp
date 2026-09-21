/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <role.h>

#include <IGlobalSlashCommand.hpp>
#include <string_view>

struct AppContext;

class WarnCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"warn"};
	static constexpr std::string_view k_description{"Warn a user. Bans the user if already warned"};

	explicit WarnCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	void warn_user(const dpp::slashcommand_t& event,
				   dpp::snowflake guild_id,
				   dpp::snowflake user_id,
				   const dpp::role& warned_role);
	dpp::cluster& discord_;
};
