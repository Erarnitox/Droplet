#pragma once

#include <Bot.hpp>

class AddResourcesCommand : public IGlobalSlashCommand {
  public:
	AddResourcesCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};