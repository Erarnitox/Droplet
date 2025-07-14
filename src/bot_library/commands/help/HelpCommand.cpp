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

#include "HelpCommand.hpp"

#include <message.h>

//-----------------------------------------------------
//
//-----------------------------------------------------
HelpCommand::HelpCommand() : IGlobalSlashCommand() {
	this->command_name = "help";
	this->command_description = "List all available commands";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void HelpCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name)
		return;

	/* create the embed */
	constexpr auto icon_url{"https://www.erarnitox.de/favicon-32x32.png"};
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::discord_black)
						 .set_title(std::format("{} - {}", "Droplet", this->command_name))
						 .set_url("https://droplet.erarnitox.de/")
						 .set_description("Usage Information for the Droplet Discord bot")
						 .set_thumbnail(icon_url)};

	for (auto& command : Bot::slash_commands) {
		const auto& cmd{command.second};

		if (not Core::is_admin(event.command.member) && cmd->command_description.ends_with("(Admin only!)")) {
			continue;
		}

		if (not Core::is_erarnitox_admin(event.command.member) &&
			cmd->command_description.ends_with("(Erarnitox only!)")) {
			continue;
		}

		const auto& options{cmd->command_options};
		std::string options_string;

		for (const auto& option : options) {
			if (option.required)
				options_string.append(std::format(" <{}>", option.name));
			else
				options_string.append(std::format(" [{}]", option.name));
		}

		embed.add_field(std::string("/").append(cmd->command_name).append(options_string), cmd->command_description);
	}

	auto footer{dpp::embed_footer()};
	footer.set_text("Visit https://erarnitox.de");
	footer.set_icon(icon_url);

	embed.set_footer(footer);

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}
