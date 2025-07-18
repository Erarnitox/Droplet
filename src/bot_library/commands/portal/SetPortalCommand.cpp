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

#include "SetPortalCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <restresults.h>
#include <snowflake.h>

#include <Core.hpp>
#include <PortalRepository.hpp>

#include "IMessageCommand.hpp"
#include "IReactionCommand.hpp"
#include "repositories/BlacklistRepository.hpp"
#include "repositories/PortalDTO.hpp"
#include "repositories/PortalRepository.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
SetPortalCommand::SetPortalCommand() : IGlobalSlashCommand(), IMessageCommand(), IReactionCommand() {
	this->command_name = "set_portal";
	this->command_description = "Set a channel as a portal for foreign messages (Admin only!)";
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (not Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto& cmd{event.command};
	const auto& guild_id{static_cast<size_t>(cmd.guild_id)};
	const auto& channel_id{static_cast<size_t>(cmd.channel_id)};

	PortalRepository repo;
	const PortalDTO data{guild_id, channel_id};

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

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_message_create(const dpp::message_create_t& event) {
	// Early returns for bots or empty messages
	if (event.msg.author.is_bot() || event.msg.content.empty()) {
		return;
	}

	PortalRepository repo;

	// Check if the message is in a registered portal channel for this guild
	if (repo.get(event.msg.guild_id).channel_id != event.msg.channel_id) {
		return;
	}

	// Check against the blacklist
	BlacklistRepository blacklist_repo;
	const auto& blacklisted_users{blacklist_repo.getAll()};
	for (const auto& user : blacklisted_users) {
		if (user.username == event.msg.author.username) {
			return;
		}
	}

	const auto send_to_portals = [event = std::move(event), repo = std::move(repo)](const std::string& prefix = "") {
		// Build the final message content
		const auto full_content = std::format("{}[**{}**]: {}", prefix, event.msg.author.username, event.msg.content);

		const auto& portals{repo.getAll()};
		for (const auto& portal : portals) {
			// Skip the channel the message came from
			if (portal.channel_id == event.msg.channel_id) {
				continue;
			}

			// FIX: Create a NEW message object for each send operation.
			dpp::message msg_to_send(portal.channel_id, full_content);
			Bot::ctx->message_create(msg_to_send);
		}
	};

	// Check if the message is a reply
	const auto& ref{event.msg.message_reference};
	if (not ref.message_id.empty()) {
		Bot::ctx->message_get(
			ref.message_id,
			ref.channel_id,
			[send_to_portals = std::move(send_to_portals)](const dpp::confirmation_callback_t& callback) {
				if (callback.is_error()) {
					send_to_portals();
					return;
				}

				const auto& ref_msg{callback.get<dpp::message>()};
				std::string reply_prefix;
				auto content = ref_msg.content.substr(0, 32);

				if (ref_msg.content.length() > 32) {
					content.append("...");
				}

				if (not ref_msg.author.is_bot()) {
					reply_prefix = std::format("> _reply to:_ `[{}]: {}`\n", ref_msg.author.username, content);
				} else if (ref_msg.author.id == Bot::ctx->me.id) {
					// Prettify replies to the bot itself
					reply_prefix = std::format("> _reply to:_ `{}`\n", content);
				}

				send_to_portals(reply_prefix);
			});
	} else {
		send_to_portals();
	}
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_message_delete(const dpp::message_delete_t& event) {
	(void)event;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	(void)event;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_message_reaction_add(const dpp::message_reaction_add_t& event) {
	(void)event;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_message_reaction_remove(const dpp::message_reaction_remove_t& event) {
	(void)event;
}
