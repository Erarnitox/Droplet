#pragma once

#include <Bot.hpp>

class WarnCommand : public IGlobalSlashCommand {
  public:
	WarnCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};
