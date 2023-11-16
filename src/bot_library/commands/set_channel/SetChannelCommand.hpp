#pragma once

#include <snowflake.h>

#include <Bot.hpp>

#include "IMemberCommand.hpp"

class SetChannelCommand : public IGlobalSlashCommand, public IMemberCommand {
  private:
	dpp::snowflake welcome_channel_id;
	dpp::snowflake goodbye_channel_id;
	dpp::snowflake log_channel_id;

  public:
	SetChannelCommand();

	virtual void on_slashcommand(const dpp::slashcommand_t& event) override;
	virtual void on_guild_member_add(const dpp::guild_member_add_t& event) override;
	virtual void on_guild_member_remove(const dpp::guild_member_remove_t& event) override;
};