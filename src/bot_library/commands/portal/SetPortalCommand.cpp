#include "SetPortalCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <message.h>
#include <snowflake.h>

#include <Core.hpp>
#include <PortalRepository.hpp>
#include <algorithm>
#include <iterator>

#include "IMessageCommand.hpp"
#include "IReactionCommand.hpp"
#include "repositories/BlacklistRepository.hpp"
#include "repositories/PortalDTO.hpp"
#include "repositories/PortalRepository.hpp"

SetPortalCommand::SetPortalCommand() : IGlobalSlashCommand(), IMessageCommand(), IReactionCommand() {
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
	// early returns
	if (event.msg.author.is_bot()) {
		return;
	} else if (event.msg.content.empty()) {
		return;
	}

	PortalRepository repo;
	BlacklistRepository blacklist_repo;

	if (repo.get(event.msg.guild_id).channel_id == event.msg.channel_id) {
		const auto& blacklisted_users{blacklist_repo.getAll()};
		for (const auto& user : blacklisted_users) {
			if (user.username == event.msg.author.username) {
				return;
			}
		}

		// cheap way to sync replies
		bool is_reply{false};
		std::string reply_string;
		const auto& ref{event.msg.message_reference};
		if (not ref.message_id.empty()) {
			auto ref_msg{Bot::ctx->message_get_sync(ref.message_id, ref.channel_id)};
			auto content = ref_msg.content.substr(0, 32);

			if (ref_msg.content.length() > 32) {
				content.append("...");
			}

			if (not ref_msg.author.is_bot()) {
				is_reply = true;
				reply_string = std::format("> _reply to:_ `[{}]: {}`\n", ref_msg.author.username, content);
			} else if (ref_msg.author.id == Bot::ctx->me.id) {
				is_reply = true;
				reply_string = std::format("> _reply to:_ `{}`\n", content);
			}
		}

		// build the message (template)
		auto msg{dpp::message(
			0,
			std::format(
				"{}[**{}**]: {}", (is_reply ? reply_string : ""), event.msg.author.username, event.msg.content))};

		for (const auto& file : event.msg.attachments) {
			const auto& file_url{file.proxy_url};
			msg.content.append(file_url);
		}

		const std::vector<PortalDTO>& portals{repo.getAll()};
		for (const PortalDTO& portal : portals) {
			if (portal.channel_id == event.msg.channel_id)
				continue;

			msg.set_channel_id(portal.channel_id);

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

void SetPortalCommand::on_message_reaction_add(const dpp::message_reaction_add_t& event) {
	(void)event;
}

void SetPortalCommand::on_message_reaction_remove(const dpp::message_reaction_remove_t& event) {
	(void)event;
}
