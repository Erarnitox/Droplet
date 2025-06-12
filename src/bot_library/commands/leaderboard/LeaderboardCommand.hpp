#pragma once

#include <Bot.hpp>

class LeaderboardCommand : public IGlobalSlashCommand {
  public:
	LeaderboardCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};