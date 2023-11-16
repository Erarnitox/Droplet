#pragma once

#include <Bot.hpp>

class ReactionRoles : public IGlobalSlashCommand, public IReactionCommand {
  public:
	ReactionRoles();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
	virtual void on_message_reaction_add(const dpp::message_reaction_add_t& event) override;
	virtual void on_message_reaction_remove(const dpp::message_reaction_remove_t& event) override;
};
