#include "ServerDetails.hpp"

#include <variant>

ServerDetails::ServerDetails() : IGlobalSlashCommand() {
	this->command_name = "server_details";
	this->command_description = "Display general server information";
}

void ServerDetails::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	const auto& guild{event.command.get_guild()};

	/* create the embed */
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::discord_black)
						 .set_title(std::format("{} Overview", guild.name))
						 .set_description(guild.description)
						 .set_thumbnail(guild.get_icon_url())};

	embed.add_field("Member Count:", std::format("{}", guild.member_count));
	embed.add_field(
		"Community:",
		std::format("{}", guild.is_community() ? "Server is a Community Server!" : "Not a Community Server!"));
	embed.add_field("Discoverable:", std::format("{}", guild.is_discoverable() ? "YES" : "NO"));
	embed.add_field("Featureable:", std::format("{}", guild.is_featureable() ? "YES" : "NO"));
	embed.add_field("Verified:", std::format("{}", guild.is_verified() ? "YES" : "NO"));

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}