#pragma once

#include <Bot.hpp>

class RollCommand : public IGlobalSlashCommand {
  public:
	RollCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};