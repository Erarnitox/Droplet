#pragma once

#include <Bot.hpp>

#include "IMessageCommand.hpp"
#include "IReactionCommand.hpp"

class SetPortalCommand : public IGlobalSlashCommand, public IMessageCommand, public IReactionCommand {
  public:
	SetPortalCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_message_create(const dpp::message_create_t& event) override;
	void on_message_delete(const dpp::message_delete_t& event) override;
	void on_message_delete_bulk(const dpp::message_delete_bulk_t& event) override;
	void on_message_reaction_add(const dpp::message_reaction_add_t& event) override;
	void on_message_reaction_remove(const dpp::message_reaction_remove_t& event) override;
};