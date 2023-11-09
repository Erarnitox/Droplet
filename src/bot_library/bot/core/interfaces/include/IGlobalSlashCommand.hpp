#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IGlobalSlashCommand {
  public:
	std::string command_name;
	std::string command_description;

	virtual void on_slashcommand(const dpp::slashcommand_t& event) = 0;
};