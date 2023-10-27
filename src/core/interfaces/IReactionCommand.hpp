#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IReactionCommand {
  public:
	virtual auto on_message_reaction_add(const dpp::message_reaction_add_t &event) -> void = 0;

	virtual auto on_message_reaction_remove(const dpp::message_reaction_remove_t &event) -> void = 0;
};