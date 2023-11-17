#pragma once

#include <Bot.hpp>

class ResourcesCommand : public IGlobalSlashCommand {
  public:
	ResourcesCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};