#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IMemberCommand {
  public:
	virtual void on_guild_member_add(const dpp::guild_member_add_t& event) = 0;

	virtual void on_guild_member_remove(const dpp::guild_member_remove_t& event) = 0;
};