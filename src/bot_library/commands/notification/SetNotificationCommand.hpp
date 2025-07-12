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

//-----------------------------------------------------
//
//-----------------------------------------------------
class SetNotificationCommand : public IGlobalSlashCommand, public IReady {
  public:
	SetNotificationCommand();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
	void on_ready(const dpp::ready_t& event) override;
};