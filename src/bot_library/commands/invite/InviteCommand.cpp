#include "InviteCommand.hpp"

#include <invite.h>
#include <message.h>

#include <format>
#include <variant>

InviteCommand::InviteCommand() : IGlobalSlashCommand() {
	this->command_name = "join_erarnitox";
	this->command_description = "Get invited to Erarnitox's discord server";
}

void InviteCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, "https://discord.erarnitox.de")
					.set_reference(event.command.id)
					.set_flags(dpp::m_ephemeral));

	return;
}