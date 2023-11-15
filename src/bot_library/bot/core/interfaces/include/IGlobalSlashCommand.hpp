#pragma once

#include <appcommand.h>
#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

#include <vector>

class IGlobalSlashCommand {
  public:
	std::string command_name;
	std::string command_description;
	std::vector<dpp::command_option> command_options;

	virtual void on_slashcommand(const dpp::slashcommand_t& event) = 0;
};