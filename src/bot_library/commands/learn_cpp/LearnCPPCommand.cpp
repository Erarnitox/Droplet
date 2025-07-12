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

//-----------------------------------------------------
//
//-----------------------------------------------------
LearnCPPCommand::LearnCPPCommand() : IGlobalSlashCommand() {
	this->command_name = "learn_cpp";
	this->command_description = "Get started learning modern C++ today!";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void LearnCPPCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

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