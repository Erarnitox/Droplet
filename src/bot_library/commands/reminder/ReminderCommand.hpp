/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IGlobalSlashCommand.hpp>
#include <string>
#include <string_view>

struct AppContext;

class ReminderCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"reminder"};
	static constexpr std::string_view k_description{"Set a reminder for yourself!"};

	explicit ReminderCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	void start_reminder(size_t channel_id,
						dpp::snowflake user_id,
						const std::string& user_mention,
						const std::string& message,
						size_t timestep_sec);
	dpp::cluster& discord_;
};
