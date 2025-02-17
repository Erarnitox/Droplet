#pragma once

#include <Bot.hpp>

#include "IMessageCommand.hpp"

class RemoveNotificationCommand : public IGlobalSlashCommand {
  public:
	RemoveNotificationCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};