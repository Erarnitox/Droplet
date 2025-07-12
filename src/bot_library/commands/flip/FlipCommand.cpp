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

#include "FlipCommand.hpp"

#include <colors.h>
#include <invite.h>
#include <message.h>

#include "UserDTO.hpp"
#include "UserRepository.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
FlipCommand::FlipCommand() : IGlobalSlashCommand() {
	this->command_name = "flip";
	this->command_description = "Flip a coin";
	this->command_options.emplace_back(dpp::co_integer, "bidding", "Bidding amount in 🌢", true);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void FlipCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	const long bidding{std::get<long>(event.get_parameter("bidding"))};
	if (bidding < 1) {
		event.reply(dpp::message("Bidding amount can't be smaller than 1").set_flags(dpp::m_ephemeral));
		return;
	}

	// Get the user sending the event
	const auto& member{event.command.member};
	if (not member.user_id) {
		event.reply(dpp::message("Can't get the member using this command. Try again!").set_flags(dpp::m_ephemeral));
		return;
	}

	// Get the user from the database
	UserRepository user_repo;
	UserDTO user_dto{};

	try {
		user_dto = user_repo.get(static_cast<size_t>(member.user_id));
	} catch (...) {
		Bot::ctx->log(dpp::ll_warning, "User is not in usr database yet");
	}

	if (static_cast<size_t>(bidding) > 9'999'999ull) {
		event.reply(dpp::message("Careful High Roller! Your Bidding is too damn high!").set_flags(dpp::m_ephemeral));
		return;
	}

	if (user_dto.exp < static_cast<size_t>(bidding)) {
		event.reply(dpp::message("Your 🌢-Balance is too low!").set_flags(dpp::m_ephemeral));
		return;
	}

	static const auto tails_url{std::string("https://www.erarnitox.de/res/tails.png")};
	static const auto heads_url{std::string("https://www.erarnitox.de/res/heads.png")};

	const auto result{std::rand() % 2};

	if (result) {
		user_dto.exp += static_cast<size_t>(bidding);
	} else {
		user_dto.exp -= static_cast<size_t>(bidding);
	}

	if (not user_repo.update(user_dto)) {
		event.reply(dpp::message("Oh no! Something went wrong! Sowwy! :c").set_flags(dpp::m_ephemeral));
		return;
	}

	/* create the embed */
	const dpp::embed embed{dpp::embed()
							   .set_color(result ? dpp::colors::green : dpp::colors::red)
							   .set_title(result ? "HEADS $_$" : "TAILS :c")
							   .set_image(result ? heads_url : tails_url)
							   .add_field("New Balance", std::format("{}🌢", user_dto.exp))};

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	return;
}