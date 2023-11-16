#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IReactionCommand {
  public:
	virtual void on_message_reaction_add(const dpp::message_reaction_add_t& event) = 0;

	virtual void on_message_reaction_remove(const dpp::message_reaction_remove_t& event) = 0;
};