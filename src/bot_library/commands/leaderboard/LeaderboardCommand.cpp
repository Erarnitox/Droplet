#include "LeaderboardCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <Core.hpp>
#include <UserRepository.hpp>

LeaderboardCommand::LeaderboardCommand() : IGlobalSlashCommand() {
	this->command_name = "leaderboard";
	this->command_description = "Get a Leaderboard of the top 10 hackers!";
}

void LeaderboardCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	UserRepository repo;

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