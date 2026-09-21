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

#include <AppContext.hpp>

SetStatus::SetStatus(AppContext& ctx) : discord_(ctx.discord) {
}

void SetStatus::on_ready(const dpp::ready_t& event) {
	(void)event;
	discord_.set_presence(dpp::presence(dpp::presence_status::ps_online,
										dpp::activity_type::at_custom,
										std::string("Empower Your Discord Community and Unleash its Full Potential")));
}