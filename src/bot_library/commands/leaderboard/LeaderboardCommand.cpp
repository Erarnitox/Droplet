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

#include "LeaderboardCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <UserDTO.hpp>
#include <UserRepository.hpp>
#include <vector>

//-----------------------------------------------------
//
//-----------------------------------------------------
LeaderboardCommand::LeaderboardCommand(AppContext& ctx) : db_(ctx.db) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void LeaderboardCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	UserRepository repo{db_};

	const std::vector<UserDTO> users = repo.getTopTen();

	/* create the embed */
	constexpr auto icon_url{"https://www.erarnitox.de/favicon-32x32.png"};
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::discord_black)
						 .set_title("TOP 10 Hackers:")
						 .set_url("https://droplet.erarnitox.de/leaderboard")
						 .set_thumbnail(icon_url)};

	for (size_t i{0}; i < 10; ++i) {
		const auto& user = users[i];
		embed.add_field(std::format("#{}: {}", (i + 1), user.user_name), std::to_string(user.exp));
	}

	auto footer{dpp::embed_footer()};
	footer.set_icon(icon_url);
	footer.set_text("Visit https://droplet.erarnitox.de/leaderboard");
	embed.set_footer(footer);

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	return;
}