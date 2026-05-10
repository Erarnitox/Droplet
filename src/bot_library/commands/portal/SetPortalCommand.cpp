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
#include <dpp/utility.h>
#include <message.h>
#include <snowflake.h>

#include <Core.hpp>
#include <PortalRepository.hpp>
#include <string_view>

#include "IMessageCommand.hpp"
#include "IReactionCommand.hpp"
#include "repositories/BlacklistRepository.hpp"
#include "repositories/PortalDTO.hpp"
#include "repositories/PortalRepository.hpp"

namespace {

constexpr std::size_t k_quote_preview_max = 160;
constexpr std::size_t k_forward_preview_max = 280;

std::string truncate_preview(std::string_view text, std::size_t max_len = k_quote_preview_max) {
	if (text.empty()) {
		return {};
	}
	if (text.size() <= max_len) {
		return std::string(text);
	}
	return std::string(text.substr(0, max_len - 3)) + "...";
}

/** Single-line preview (quoted banners can't rely on multi-line markdown in relay messages). */
std::string flatten_preview(std::string text) {
	for (char& c : text) {
		if (c == '\n' || c == '\r') {
			c = ' ';
		}
	}
	return text;
}

std::string display_name_for_message(const dpp::message& m) {
	const std::string nick = m.member.get_nickname();
	if (!nick.empty()) {
		return nick;
	}
	if (!m.author.global_name.empty()) {
		return std::string(m.author.global_name);
	}
	return m.author.username;
}

/** Short note when the original had attachments, embeds, or stickers. */
std::string media_annotation(const dpp::message& m) {
	std::string out;
	if (!m.attachments.empty()) {
		out += std::format(" · {} attachment(s)", m.attachments.size());
	}
	if (!m.embeds.empty()) {
		out += " · embed";
	}
	if (!m.stickers.empty()) {
		out += " · sticker";
	}
	return out;
}

/** Markdown link to open the message in Discord (needs guild + channel + message id). */
std::string jump_markdown_for_message(const dpp::message& target, const dpp::message& id_fallback) {
	const dpp::snowflake gid = target.guild_id.empty() ? id_fallback.guild_id : target.guild_id;
	const dpp::snowflake cid = target.channel_id.empty() ? id_fallback.channel_id : target.channel_id;
	const dpp::snowflake mid = target.id;
	const std::string url = dpp::utility::message_url(gid, cid, mid);
	if (url.empty()) {
		return {};
	}
	return std::format(" · [Jump]({})", url);
}

bool message_has_relayable_payload(const dpp::message& msg) {
	if (!msg.content.empty()) {
		return true;
	}
	if (!msg.attachments.empty() || !msg.embeds.empty() || !msg.stickers.empty()) {
		return true;
	}
	if (msg.message_reference.type == dpp::mrt_forward && !msg.message_snapshots.messages.empty()) {
		return true;
	}
	return false;
}

std::string format_forwarded_snapshots_block(const dpp::message& msg) {
	if (msg.message_reference.type != dpp::mrt_forward || msg.message_snapshots.messages.empty()) {
		return {};
	}

	std::string block = "📨 **Forwarded**\n";
	for (const auto& inner : msg.message_snapshots.messages) {
		const std::string who = display_name_for_message(inner);
		std::string body =
			inner.content.empty() ? std::string("(no text)") : truncate_preview(inner.content, k_forward_preview_max);
		body = flatten_preview(std::move(body));
		const std::string jump = jump_markdown_for_message(inner, msg);
		block += std::format("> **{}**: {}{}{}\n", who, body, media_annotation(inner), jump);
	}
	return block;
}

std::string format_reply_banner(const dpp::message& ref_msg, const dpp::message& portal_context_msg) {
	const std::string who = display_name_for_message(ref_msg);
	std::string preview =
		ref_msg.content.empty() ? std::string("(no text)") : truncate_preview(ref_msg.content, k_quote_preview_max);
	preview = flatten_preview(std::move(preview));

	const std::string jump = jump_markdown_for_message(ref_msg, portal_context_msg);

	if (ref_msg.author.id == Bot::ctx->me.id) {
		return std::format("↩️ _Reply to portal relay:_ {}{}{}\n", preview, media_annotation(ref_msg), jump);
	}
	return std::format("↩️ _Reply to_ **{}**: {}{}{}\n", who, preview, media_annotation(ref_msg), jump);
}

/** Bottom line: who spoke (and optional commentary / media on the outer message). */
std::string format_outgoing_body_line(const dpp::message& msg) {
	const std::string who = display_name_for_message(msg);
	const bool forwarded = (msg.message_reference.type == dpp::mrt_forward);

	const std::string jump = jump_markdown_for_message(msg, msg);

	if (forwarded && msg.content.empty()) {
		return std::format("_**{}** forwarded the message above._{}{}", who, media_annotation(msg), jump);
	}

	if (msg.content.empty()) {
		return std::format("**{}**{}{}", who, media_annotation(msg), jump);
	}

	return std::format("**{}**: {}", who, msg.content) + media_annotation(msg) + jump;
}

}  // namespace

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
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void SetPortalCommand::on_message_create(const dpp::message_create_t& event) {
	if (event.msg.author.is_bot() || not message_has_relayable_payload(event.msg)) {
		return;
	}

	PortalRepository repo;

	if (repo.get(event.msg.guild_id).channel_id != event.msg.channel_id) {
		return;
	}

	BlacklistRepository blacklist_repo;
	const auto& blacklisted_users{blacklist_repo.getAll()};
	for (const auto& user : blacklisted_users) {
		if (user.username == event.msg.author.username) {
			return;
		}
	}

	const std::string forward_banner = format_forwarded_snapshots_block(event.msg);

	const auto send_to_portals =
		[event = std::move(event), repo = std::move(repo), forward_banner](std::string reply_banner) {
			const std::string header = std::format("{}{}", forward_banner, reply_banner);
			const std::string body_line = format_outgoing_body_line(event.msg);
			const std::string full_content = header.empty() ? body_line : std::format("{}\n{}", header, body_line);

			const auto& portals{repo.getAll()};
			for (const auto& portal : portals) {
				if (portal.channel_id == event.msg.channel_id) {
					continue;
				}

				dpp::message msg_to_send(portal.channel_id, full_content);
				Bot::ctx->message_create(msg_to_send);
			}
		};

	const auto& ref{event.msg.message_reference};
	const bool is_forward = (ref.type == dpp::mrt_forward);
	const bool needs_reply_lookup = not ref.message_id.empty() && not is_forward;

	if (needs_reply_lookup) {
		const dpp::snowflake ref_channel_id = ref.channel_id.empty() ? event.msg.channel_id : ref.channel_id;
		const dpp::message portal_source_for_links = event.msg;
		const dpp::snowflake reply_message_id = ref.message_id;
		const dpp::snowflake guild_for_jump = event.msg.guild_id;
		Bot::ctx->message_get(
			ref.message_id,
			ref_channel_id,
			[send_to_portals = std::move(send_to_portals),
			 portal_source_for_links,
			 reply_message_id,
			 ref_channel_id,
			 guild_for_jump](const dpp::confirmation_callback_t& callback) {
				if (callback.is_error()) {
					const std::string url = dpp::utility::message_url(guild_for_jump, ref_channel_id, reply_message_id);
					if (not url.empty()) {
						send_to_portals(std::format("↩️ _Reply (preview unavailable)_ · [Jump]({})\n", url));
					} else {
						send_to_portals("");
					}
					return;
				}

				send_to_portals(format_reply_banner(callback.get<dpp::message>(), portal_source_for_links));
			});
	} else {
		send_to_portals("");
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
