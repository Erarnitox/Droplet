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

class RollCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"roll"};
	static constexpr std::string_view k_description{"Roll a dice"};

	explicit RollCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
};
