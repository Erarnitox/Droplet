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

class InviteCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"join_erarnitox"};
	static constexpr std::string_view k_description{"Get invited to Erarnitox's discord server"};

	explicit InviteCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};
