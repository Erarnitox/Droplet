#pragma once

#include <dispatcher.h>

#include <Bot.hpp>

class ReminderCommand : public IGlobalSlashCommand {
  public:
	ReminderCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};