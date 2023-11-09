#pragma once

#include <Bot.hpp>

class HelpCommand : public IGlobalSlashCommand {
  public:
	HelpCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};