#pragma once

#include <Bot.hpp>

class ProfileCommand : public IGlobalSlashCommand {
  public:
	ProfileCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};