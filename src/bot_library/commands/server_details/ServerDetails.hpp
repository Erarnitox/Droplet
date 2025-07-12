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

#include <Bot.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
class ServerDetails : public IGlobalSlashCommand {
  public:
	ServerDetails();

	void on_slashcommand(const dpp::slashcommand_t& event) override;
};