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

#include <AppContext.hpp>
#include <cstdint>
#include <format>
#include <random>
#include <string>
#include <string_view>

#include "UserDTO.hpp"
#include "UserRepository.hpp"

namespace {
std::mt19937& rng() {
	static thread_local std::mt19937 gen{std::random_device{}()};
	return gen;
}
}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
FlipCommand::FlipCommand(AppContext& ctx) : discord_(ctx.discord), db_(ctx.db) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->command_options.emplace_back(dpp::co_integer, "bidding", "Bidding amount in 🌢", true);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void FlipCommand::on_slashcommand(const dpp::slashcommand_t& event) {
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
	UserRepository user_repo{db_};
	UserDTO user_dto{};

	try {
		user_dto = user_repo.get(static_cast<size_t>(member.user_id));
	} catch (...) {
		discord_.log(dpp::ll_warning, "User is not in usr database yet");
	}

	if (static_cast<size_t>(bidding) > 9'999'999ull) {
		event.reply(dpp::message("Careful High Roller! Your Bidding is too damn high!").set_flags(dpp::m_ephemeral));
		return;
	}

	if (user_dto.exp < static_cast<size_t>(bidding)) {
		event.reply(dpp::message("Your 🌢-Balance is too low!").set_flags(dpp::m_ephemeral));
		return;
	}

	constexpr std::string_view tails_url{"https://www.erarnitox.de/res/tails.png"};
	constexpr std::string_view heads_url{"https://www.erarnitox.de/res/heads.png"};

	std::uniform_int_distribution<int> dist(0, 1);
	const auto result{dist(rng())};
	const auto bid{static_cast<size_t>(bidding)};
	const std::int64_t delta{result ? static_cast<std::int64_t>(bid) : -static_cast<std::int64_t>(bid)};
	const auto new_exp{user_repo.try_adjust_exp(static_cast<size_t>(member.user_id), delta, bid)};
	if (not new_exp) {
		event.reply(dpp::message("Your 🌢-Balance is too low!").set_flags(dpp::m_ephemeral));
		return;
	}

	/* create the embed */
	const dpp::embed embed{dpp::embed()
							   .set_color(result ? dpp::colors::green : dpp::colors::red)
							   .set_title(result ? "HEADS $_$" : "TAILS :c")
							   .set_image(std::string(result ? heads_url : tails_url))
							   .add_field("New Balance", std::format("{}🌢", *new_exp))};

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	return;
}