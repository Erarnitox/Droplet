#pragma once

#include <Bot.hpp>

class LearnCPPCommand : public IGlobalSlashCommand {
  public:
	LearnCPPCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};