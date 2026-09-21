/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IGlobalSlashCommand.hpp>
#include <IReady.hpp>
#include <string_view>

struct AppContext;
class DatabaseExecutor;
class YoutubeNotificationService;

class SetNotificationCommand : public IGlobalSlashCommand, public IReady {
  public:
	static constexpr std::string_view k_name{"youtube_uploads"};
	static constexpr std::string_view k_description{"Get notifications about youtube uploads to discord (Admin only!)"};

	explicit SetNotificationCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_ready(const dpp::ready_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
	YoutubeNotificationService& youtube_;
};
