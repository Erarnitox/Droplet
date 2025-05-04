#pragma once

#include <Bot.hpp>

class GetResourcesCommand : public IGlobalSlashCommand {
  public:
	GetResourcesCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};