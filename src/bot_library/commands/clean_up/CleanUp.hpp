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
class CleanUp : public IMessageCommand {
  public:
	virtual void on_message_create(const dpp::message_create_t& event) override;

	virtual void on_message_delete(const dpp::message_delete_t& event) override;

	virtual void on_message_delete_bulk(const dpp::message_delete_bulk_t& event) override;
};