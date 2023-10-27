#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IMemberCommand {
  public:
	virtual auto on_guild_member_add(const dpp::guild_member_add_t& event) -> void = 0;

	virtual auto on_guild_member_remove(const dpp::guild_member_remove_t& event) -> void = 0;
};