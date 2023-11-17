#include "FlipCommand.hpp"

#include <invite.h>
#include <message.h>

#include <format>
#include <random>
#include <variant>

FlipCommand::FlipCommand() : IGlobalSlashCommand() {
	this->command_name = "flip";
	this->command_description = "Flip a coin";
}

void FlipCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	static auto tails_url{std::string("https://www.erarnitox.de/res/tails.png")};
	static auto heads_url{std::string("https://www.erarnitox.de/res/heads.png")};

	const auto result{std::rand() % 2};

	/* reply with the created embed */
	event.reply(
		dpp::message(event.command.channel_id, (result ? heads_url : tails_url)).set_reference(event.command.id));

	return;
}