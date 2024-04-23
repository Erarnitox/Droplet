#include "SetPortalCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <Core.hpp>
#include <PortalRepository.hpp>

#include "IMessageCommand.hpp"
#include "repositories/PortalDTO.hpp"
#include "repositories/PortalRepository.hpp"

SetPortalCommand::SetPortalCommand() : IGlobalSlashCommand(), IMessageCommand() {
	this->command_name = "set_portal";
	this->command_description = "Set a channel as a portal for foreign messages (Admin only!)";
}

void SetPortalCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};
	const auto& channel_id{static_cast<size_t>(cmd.channel_id)};

	PortalRepository repo;
	PortalDTO data{guild_id, channel_id};

	if (repo.get(data.guild_id).channel_id != 0) {
		if (repo.update(data)) {
			auto msg{dpp::message("Portal was moved here!")};
			event.reply(msg);
		} else {
			event.reply(dpp::message("Error: Failed to update the Portal!").set_flags(dpp::m_ephemeral));
		}
	} else if (repo.create(data)) {
		auto msg{dpp::message("Portal enabled! ...")};
		event.reply(msg);
	} else {
		event.reply(dpp::message("Error: Failed to save Portal to the Database!").set_flags(dpp::m_ephemeral));
	}
	return;

	event.reply(dpp::message("Error: Something went wrong!").set_flags(dpp::m_ephemeral));
	return;
}

void SetPortalCommand::on_message_create(const dpp::message_create_t& event) {
	if (event.msg.author.is_bot())
		return;

	PortalRepository repo;
	if (repo.get(event.msg.guild_id).channel_id == event.msg.channel_id) {
		const std::vector<PortalDTO>& portals{repo.getAll()};
		for (const PortalDTO& portal : portals) {
			if (portal.channel_id == event.msg.channel_id)
				continue;

			const auto& msg{dpp::message(portal.channel_id,
										 std::format("**{}**: {}", event.msg.author.username, event.msg.content))};
			Bot::ctx->message_create(msg);
		}
	}
}

void SetPortalCommand::on_message_delete(const dpp::message_delete_t& event) {
	(void)event;
}

void SetPortalCommand::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	(void)event;
}