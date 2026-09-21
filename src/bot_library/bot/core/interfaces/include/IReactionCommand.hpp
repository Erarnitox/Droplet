#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IReactionCommand {
  public:
	virtual ~IReactionCommand() = default;

	virtual void on_message_reaction_add(const dpp::message_reaction_add_t& event) {
		(void)event;
	}

	virtual void on_message_reaction_remove(const dpp::message_reaction_remove_t& event) {
		(void)event;
	}
};
