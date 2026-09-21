/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IFormCommand.hpp>
#include <IGlobalSlashCommand.hpp>
#include <string_view>

struct AppContext;

class BugReportCommand : public IGlobalSlashCommand, public IFormCommand {
  public:
	static constexpr std::string_view k_name{"report_bug"};
	static constexpr std::string_view k_description{"Report a bug to the developers"};

	explicit BugReportCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_form_submit(const dpp::form_submit_t& event) override;

  private:
	dpp::cluster& discord_;
};
