#pragma once

#include <Bot.hpp>

class FlipCommand : public IGlobalSlashCommand {
  public:
	FlipCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};