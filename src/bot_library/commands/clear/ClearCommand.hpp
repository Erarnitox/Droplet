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

class ClearCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"clear"};
	static constexpr std::string_view k_description{"clear the current channel completely"};

	explicit ClearCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	dpp::cluster& discord_;
};
