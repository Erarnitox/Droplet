#include "RollCommand.hpp"

#include <invite.h>
#include <message.h>

#include <format>
#include <random>
#include <variant>

RollCommand::RollCommand() : IGlobalSlashCommand() {
	this->command_name = "roll";
	this->command_description = "Roll a dice";
}

void RollCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	static std::string number_urls[]{std::string("https://erarnitox.de/res/dice/1.png"),
									 std::string("https://erarnitox.de/res/dice/2.png"),
									 std::string("https://erarnitox.de/res/dice/3.png"),
									 std::string("https://erarnitox.de/res/dice/4.png"),
									 std::string("https://erarnitox.de/res/dice/5.png"),
									 std::string("https://erarnitox.de/res/dice/6.png")};

	const auto result{std::rand() % 6};

	event.reply(dpp::message(event.command.channel_id, number_urls[result]).set_reference(event.command.id));

	return;
}