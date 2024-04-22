#pragma once

#include <Bot.hpp>

class ProductsCommand : public IGlobalSlashCommand {
  public:
	ProductsCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
};