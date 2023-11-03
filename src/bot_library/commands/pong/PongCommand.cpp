/**
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *  
 *  License: MIT License
 *
 *  Description: A very simple ping/pong message command
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/pongcommand
**/

#include "PongCommand.hpp"

/**
* @brief implement an on message create handler that respons with "pong"
*
* @param event the message send/create event
* @return doesn't return anything
*/
void PongCommand::on_message_create(const dpp::message_create_t& event) {
	if (event.msg.content.starts_with("ping")) {
		event.reply("Pong!");
	}
}

/**
* @brief implements an empty message delete handler
*
* @param event the message delete event
* @return doesn't return anything
*/
void PongCommand::on_message_delete(const dpp::message_delete_t& event) {
	(void)event;
}

/**
* @brief implements an empty message delete bulk handler
*
* @param event the message delete bulk event
* @return doesn't return anything
*/
void PongCommand::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	(void)event;
}