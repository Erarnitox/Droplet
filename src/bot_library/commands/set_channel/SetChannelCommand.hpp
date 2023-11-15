#pragma once

#include <Bot.hpp>

#include "IMemberCommand.hpp"

class SetChannelCommand : public IGlobalSlashCommand {
  public:
	SetChannelCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};