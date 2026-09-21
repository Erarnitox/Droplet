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

class LeaderboardCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"leaderboard"};
	static constexpr std::string_view k_description{"Get a Leaderboard of the top 10 hackers!"};

	explicit LeaderboardCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	DatabaseExecutor& db_;
};
