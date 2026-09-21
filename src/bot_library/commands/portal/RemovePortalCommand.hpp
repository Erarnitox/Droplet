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
class PortalIndex;

class RemovePortalCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"remove_portal"};
	static constexpr std::string_view k_description{"Remove the portal from this server (Admin only!)"};

	explicit RemovePortalCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	DatabaseExecutor& db_;
	PortalIndex& portal_index_;
};
