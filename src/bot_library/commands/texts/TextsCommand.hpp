#pragma once

#include <Bot.hpp>

class TextsCommand : public IGlobalSlashCommand {
  public:
	TextsCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};