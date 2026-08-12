/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#pragma once

#include <dispatcher.h>

#include <Bot.hpp>
#include <string>

/** Stop the youtube upload poller for \p key (`{discord_channel_id}/{youtube_channel_id}`). */
void stop_youtube_notification_daemon(const std::string& key);

//-----------------------------------------------------
//
//-----------------------------------------------------
class SetNotificationCommand : public IGlobalSlashCommand, public IReady {
  public:
	SetNotificationCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_ready(const dpp::ready_t& event) override;
};