#include "HelpCommand.hpp"

#include <message.h>

HelpCommand::HelpCommand() : IGlobalSlashCommand() {
	this->command_name = "help";
	this->command_description = "List all available commands";
}

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

		if (!Core::is_admin(event.command.member) && cmd->command_description.ends_with("(Admin only!)")) {
			continue;
		}

		if (!Core::is_erarnitox_admin(event.command.member) &&
			cmd->command_description.ends_with("(Erarnitox only!)")) {
			continue;
		}

		auto& options{cmd->command_options};
		std::string options_string;

		for (auto& option : options) {
			if (option.required)
				options_string.append(std::format(" <{}>", option.name));
			else
				options_string.append(std::format(" [{}]", option.name));
		}

		embed.add_field(std::string("/").append(cmd->command_name).append(options_string), cmd->command_description);
	}

	/*
	//TODO: figure out how to have hyperlinks in the footer
	- make a tutorial video about droplet
	- write a usage guide and link it here
	constexpr auto invite_link{
		"https://discord.com/api/oauth2/authorize?client_id=1071149612305498162&permissions=8&scope=bot"};
	constexpr auto usage_guide{"https://droplet.erarnitox.de/guide.html"};
	constexpr auto erarnitox_link{"https://erarnitox.de"};
	*/
	auto footer{dpp::embed_footer()};
	/*
	footer.set_text(std::format(
		"[Invite this Bot]({}) [Usage Manual]({}) [Visit Erarnitox]({})", invite_link, usage_guide, erarnitox_link));
	*/
	footer.set_text("Visit https://erarnitox.de");
	footer.set_icon(icon_url);

	embed.set_footer(footer);

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
	// Bot::ctx->message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}
