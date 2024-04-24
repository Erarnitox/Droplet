#pragma once

#include <Bot.hpp>

#include "IMessageCommand.hpp"

class BlacklistCommand : public IGlobalSlashCommand {
  public:
	BlacklistCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};