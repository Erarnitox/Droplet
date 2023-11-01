#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IGlobalSlashCommand {
  public:
	std::string command_name;
	std::string command_description;

	virtual auto create_slashcommand(const dpp::ready_t& event) -> void = 0;

	virtual auto on_slashcommand(const dpp::slashcommand_t& event) -> void = 0;
};