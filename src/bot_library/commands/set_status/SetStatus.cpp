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

#include "SetStatus.hpp"

#include <presence.h>

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetStatus::on_ready(const dpp::ready_t& event) {
	(void)event;

	Bot::ctx->set_presence(dpp::presence(dpp::presence_status::ps_online,
										 dpp::activity_type::at_custom,
										 std::string("Empower Your Discord Community and Unleash its Full Potential")));

	return;
}