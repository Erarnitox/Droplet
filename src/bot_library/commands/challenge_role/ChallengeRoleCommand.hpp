/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IButtonCommand.hpp>
#include <IFormCommand.hpp>
#include <IGlobalSlashCommand.hpp>
#include <string_view>

struct AppContext;
class DatabaseExecutor;

class ChallengeRoleCommand : public IGlobalSlashCommand, public IButtonCommand, public IFormCommand {
  public:
	static constexpr std::string_view k_name{"challenge_role"};
	static constexpr std::string_view k_description{"Create challenge Roles (Admin only!)"};

	explicit ChallengeRoleCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_button_click(const dpp::button_click_t& event) override;
	void on_form_submit(const dpp::form_submit_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
};
