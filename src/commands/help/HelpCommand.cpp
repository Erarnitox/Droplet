#include "HelpCommand.hpp"

#include <fmt/core.h>

#include <Commands.hpp>
#include <Core.hpp>
#include <variant>

auto HelpCommand::registerGlobalSlashCommand(std::vector<dpp::slashcommand> &command_list,
											 const dpp::cluster &bot) noexcept -> void {
	dpp::slashcommand help_command("help", "Usage information", bot.me.id);
	command_list.push_back(help_command);
}

auto HelpCommand::handleGlobalSlashCommand(const dpp::slashcommand_t &event,
										   dpp::cluster &bot,
										   const std::vector<dpp::slashcommand> &command_list) noexcept -> void {
	// unneeded arguments:
	(void)command_list;

	if (event.command.get_command_name() != "help")
		return;

	/* create the embed */
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::discord_black)
						 .set_title("Droplet - Help")  // TODO: generate dynamically
						 .set_url("https://droplet.dropsoft.org/")
						 .set_description("Usage Information for the Droplet Discord bot")
						 .set_thumbnail("https://www.dropsoft.org/img/"
										"logo_huc21a5771e65b8d5ba9ff88b74b45cd86_105986_"
										"288x288_fill_box_center_3.png")};

	for (auto &command : command_list) {
		if (!Core::isAdmin(event.command.member) && command.description.ends_with("(Admin only!)"))
			continue;

		auto &options{command.options};
		std::string options_string;

		for (auto &option : options) {
			if (option.required)
				options_string.append(fmt::format(" <{}>", option.name));
			else
				options_string.append(fmt::format(" [{}]", option.name));
		}

		embed.add_field(std::string("/").append(command.name).append(options_string), command.description);
	}

	/* reply with the created embed */
	event.reply("Here is the usage manual!");
	bot.message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
}

auto HelpCommand::handleButtonClicks(const dpp::button_click_t &event, dpp::cluster &bot) noexcept -> void {
	(void)event;
	(void)bot;
}

auto HelpCommand::handleFormSubmits(const dpp::form_submit_t &event, dpp::cluster &bot) noexcept -> void {
	(void)event;
	(void)bot;
}

// user management
auto HelpCommand::welcomeMember(const dpp::guild_member_add_t &event, dpp::cluster &bot) -> void {
	(void)event;
	(void)bot;
};

auto HelpCommand::leaveMember(const dpp::guild_member_remove_t &event, dpp::cluster &bot) -> void {
	(void)event;
	(void)bot;
}

// handle added reactions
auto HelpCommand::handleReactionAdded(const dpp::message_reaction_add_t &event, dpp::cluster &bot) -> void {
	(void)event;
	(void)bot;
}

// handle removed reactions
auto HelpCommand::handleReactionRemoved(const dpp::message_reaction_remove_t &event, dpp::cluster &bot) -> void {
	(void)event;
	(void)bot;
}