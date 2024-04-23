#pragma once

#include <Bot.hpp>

#include "IMessageCommand.hpp"

class RemovePortalCommand : public IGlobalSlashCommand {
  public:
	RemovePortalCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};