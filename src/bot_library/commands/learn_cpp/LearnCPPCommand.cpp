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

#include "LearnCPPCommand.hpp"

#include <message.h>

#include <AppContext.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
LearnCPPCommand::LearnCPPCommand(AppContext& ctx) {
	(void)ctx;
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void LearnCPPCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id,
							 "If you have always wanted to learn C++\n"
							 "then now is the time with Erarnitox's Playlist on youtube:\n"
							 "https://www.youtube.com/watch?v=2O3r5qqjyiY&list=PLTjUlazALHSBQp4jdqHTCduTSSMU-cz5P\n\n"
							 "Please do also read the accompanying article on his website:\n"
							 "https://www.erarnitox.de/pub/cpp_tut/")
					.set_reference(event.command.id));

	return;
}