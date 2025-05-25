#pragma once

#include <Bot.hpp>

class BugReportCommand : public IGlobalSlashCommand, public IFormCommand {
  public:
	BugReportCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_form_submit(const dpp::form_submit_t& event) override;
};
