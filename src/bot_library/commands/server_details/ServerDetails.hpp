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

class ServerDetails : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"server_details"};
	static constexpr std::string_view k_description{"Display general server information"};

	explicit ServerDetails(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};
