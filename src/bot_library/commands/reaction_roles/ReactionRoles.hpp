/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IGlobalSlashCommand.hpp>
#include <IReactionCommand.hpp>
#include <string_view>

struct AppContext;
class DatabaseExecutor;

class ReactionRoles : public IGlobalSlashCommand, public IReactionCommand {
  public:
	static constexpr std::string_view k_name{"reaction_role"};
	static constexpr std::string_view k_description{"Create reaction Roles (Admin only!)"};

	explicit ReactionRoles(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_message_reaction_add(const dpp::message_reaction_add_t& event) override;
	void on_message_reaction_remove(const dpp::message_reaction_remove_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
};
