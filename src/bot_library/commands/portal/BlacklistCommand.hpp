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
class BlacklistIndex;

class BlacklistCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"blacklist"};
	static constexpr std::string_view k_description{"Blacklist a username using the portal (Erarnitox only!)"};

	explicit BlacklistCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
	BlacklistIndex& blacklist_index_;
};
