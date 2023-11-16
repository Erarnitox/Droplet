#pragma once

#include <Bot.hpp>

class ServerDetails : public IGlobalSlashCommand {
  public:
	ServerDetails();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};