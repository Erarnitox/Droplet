#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IGlobalSlashCommand {
  public:
	virtual ~IGlobalSlashCommand() = default;

	std::string command_name;
	std::string command_description;
	std::vector<dpp::command_option> command_options;
	bool admin_only{false};

	virtual void on_slashcommand(const dpp::slashcommand_t& event) = 0;
};
