#include "HelpCommand.hpp"

#include <variant>

HelpCommand::HelpCommand() : IGlobalSlashCommand() {
	this->command_name = "help";
	this->command_description = "List all available commands";
}

void HelpCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name)
		return;

	/* create the embed */
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::discord_black)
						 .set_title(std::format("{} - {}", "Droplet", this->command_name))
						 .set_url("https://droplet.dropsoft.org/")
						 .set_description("Usage Information for the Droplet Discord bot")
						 .set_thumbnail("https://www.dropsoft.org/img/"
										"logo_huc21a5771e65b8d5ba9ff88b74b45cd86_105986_"
										"288x288_fill_box_center_3.png")};

	for (auto& command : Bot::slash_commands) {
		const auto& cmd{command.second};
		if (!Core::is_admin(event.command.member) && cmd->command_description.ends_with("(Admin only!)")) {
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

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
	// Bot::ctx->message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}