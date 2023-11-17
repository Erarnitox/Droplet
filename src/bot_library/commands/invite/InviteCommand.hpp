#pragma once

#include <Bot.hpp>

class InviteCommand : public IGlobalSlashCommand {
  public:
	InviteCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};