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

class AddResourcesCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"add_resource"};
	static constexpr std::string_view k_description{"Add resource to droplet.erarnitox.de/resources"};

	explicit AddResourcesCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
};
