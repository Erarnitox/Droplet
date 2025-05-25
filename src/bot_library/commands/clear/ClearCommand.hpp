#pragma once

#include <Bot.hpp>

class ClearCommand : public IGlobalSlashCommand {
  public:
	ClearCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};
