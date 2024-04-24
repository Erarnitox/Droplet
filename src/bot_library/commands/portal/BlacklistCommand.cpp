#include "BlacklistCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <BlacklistDTO.hpp>
#include <BlacklistRepository.hpp>
#include <Core.hpp>

#include "repositories/BlacklistDTO.hpp"
#include "repositories/BlacklistRepository.hpp"

BlacklistCommand::BlacklistCommand() : IGlobalSlashCommand() {
	this->command_name = "blacklist";
	this->command_description = "Blacklist a username using the portal (Dropsoft only!)";

	this->command_options.emplace_back(dpp::co_string, "username", "The username to be blacklisted", true);
}

void BlacklistCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_dropsoft_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto username{Core::get_parameter(*Bot::ctx, event, "username")};

	BlacklistRepository repo;
	BlacklistDTO dto;

	const auto& blacklisted_users{repo.getAll()};
	for (const auto& user : blacklisted_users) {
		if (user.username == username) {
			return;
		}
	}

	dto.username = username;
	if (repo.create(dto)) {
		event.reply(dpp::message("User was added to the blacklist!").set_flags(dpp::m_ephemeral));
	} else {
		event.reply(dpp::message("Something went wrong!").set_flags(dpp::m_ephemeral));
	}

	return;
}