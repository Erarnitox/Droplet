/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <CommandTypes.hpp>
#include <IGlobalSlashCommand.hpp>
#include <string_view>

struct AppContext;

class HelpCommand : public IGlobalSlashCommand {
  public:
	static constexpr std::string_view k_name{"help"};
	static constexpr std::string_view k_description{"List all available commands"};

	explicit HelpCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;

  private:
	slash_commands_t& slash_commands_;
};
