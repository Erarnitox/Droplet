/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IGlobalSlashCommand.hpp>
#include <IMessageCommand.hpp>
#include <string_view>

struct AppContext;
class DatabaseExecutor;
class PortalIndex;
class BlacklistIndex;

class SetPortalCommand : public IGlobalSlashCommand, public IMessageCommand {
  public:
	static constexpr std::string_view k_name{"set_portal"};
	static constexpr std::string_view k_description{"Set a channel as a portal for foreign messages (Admin only!)"};

	explicit SetPortalCommand(AppContext& ctx);

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_message_create(const dpp::message_create_t& event) override;

  private:
	dpp::cluster& discord_;
	DatabaseExecutor& db_;
	PortalIndex& portal_index_;
	BlacklistIndex& blacklist_index_;
};
