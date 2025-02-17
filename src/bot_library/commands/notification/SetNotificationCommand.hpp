#pragma once

#include <dispatcher.h>

#include <Bot.hpp>

class SetNotificationCommand : public IGlobalSlashCommand, public IReady {
  public:
	SetNotificationCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_ready(const dpp::ready_t& event) override;
};