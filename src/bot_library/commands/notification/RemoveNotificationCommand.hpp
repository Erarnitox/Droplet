/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IGlobalSlashCommand.hpp>
#include <string_view>

struct AppContext;
class DatabaseExecutor;
class YoutubeNotificationService;

class RemoveNotificationCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"remove_notification"};
	static constexpr std::string_view k_description{"Remove notification events from this server (Admin only!)"};

	explicit RemoveNotificationCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
	YoutubeNotificationService& youtube_;
};
