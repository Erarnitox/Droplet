#pragma once

#include <Bot.hpp>
#include <stack>
#include <stdexcept>
#include <string>

class ChallengeBadgeCommand : public IGlobalSlashCommand, public IButtonCommand, public IFormCommand {
  public:
	ChallengeBadgeCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
	virtual void on_button_click(const dpp::button_click_t& event) override;
	virtual void on_form_submit(const dpp::form_submit_t& event) override;
};