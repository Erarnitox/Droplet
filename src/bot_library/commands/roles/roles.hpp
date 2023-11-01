#pragma once

#include <dpp/colors.h>
#include <dpp/message.h>
#include <dpp/restresults.h>
#include <fmt/core.h>

#include <Core.hpp>
#include <Database.hpp>
#include <stack>
#include <stdexcept>
#include <string>

namespace roles {
static inline auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list,
												  const dpp::cluster& bot) noexcept -> void {
	// Reaction Role
	dpp::slashcommand reaction_role("reaction_role", "Create reaction Roles (Admin only!)", bot.me.id);

	reaction_role.add_option(
		dpp::command_option(dpp::co_string, "message_link", "A link to the message to react to", true));

	reaction_role.add_option(
		dpp::command_option(dpp::co_string, "emoji", "The emoji the user needs to react with", true));

	reaction_role.add_option(dpp::command_option(dpp::co_string, "role", "The role that will be granted", true));

	command_list.push_back(reaction_role);
}

static inline auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) noexcept -> void {
	if (event.command.get_command_name() == "reaction_role") {
		if (!Core::isAdmin(event.command.member)) {
			event.reply(dpp::message("Only admins are allowed to use this command!").set_flags(dpp::m_ephemeral));
			return;
		}

		const auto message_link{Core::getParameter(bot, event, "message_link")};
		if (message_link.empty())
			return;

		const auto emoji{Core::getParameter(bot, event, "emoji")};
		if (emoji.empty())
			return;

		const auto role{Core::getParameter(bot, event, "role")};
		if (role.empty())
			return;

		const auto role_id{Core::getRoleId(role)};
		if (role_id.empty()) {
			event.reply(dpp::message("No valid Role was provided!").set_flags(dpp::m_ephemeral));
			return;
		}

		const auto usable_emoji{emoji.starts_with("<:") ? emoji.substr(2, emoji.size() - 3) : emoji};
		if (usable_emoji.empty()) {
			event.reply(dpp::message("No valid emoji was provided!").set_flags(dpp::m_ephemeral));
			return;
		}

		// indecies of slashes in the link
		std::vector<size_t> slashes;

		if (message_link.size() < 4 || !message_link.starts_with("http")) {
			event.reply(dpp::message("The provided link is not a valid message link!").set_flags(dpp::m_ephemeral));
			return;
		}

		// find the position of all slashes
		for (size_t i{0};;) {
			i = message_link.find("/", i + 1);
			if (i == std::string::npos)
				break;
			slashes.push_back(i);
		}

		if (slashes.size() < 3) {
			event.reply("The provided link is not a valid message link!");
			return;
		}

		//--------------------------------------------------
		// get message id from the link
		size_t end_pos{message_link.size()};
		size_t start_pos{slashes.back() + 1};
		const auto& message_id{message_link.substr(start_pos, end_pos)};
		//--------------------------------------------------

		//--------------------------------------------------
		// get message id from the link
		slashes.pop_back();
		end_pos = (start_pos - 2) - slashes.back();
		start_pos = slashes.back() + 1;
		const auto& channel_id{message_link.substr(start_pos, end_pos)};
		//--------------------------------------------------

		// Insert Reaction Role into Database
		Database::insert_reaction_role_data(role_id, event.command.guild_id, message_id, usable_emoji);

		// Let the bot react to the message
		bot.message_add_reaction(message_id, channel_id, usable_emoji);

		// send a confirmation to the admin
		event.reply(
			dpp::message(
				fmt::format("Reaction Role Created!\nMessage: {}\nReaction: {}\nRole: {}", message_link, emoji, role))
				.set_flags(dpp::m_ephemeral));
	}
}

static inline auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) noexcept -> void {
	const auto message_id{event.message_id};
	if (!message_id)
		return;

	const auto user_id{event.reacting_user.id};
	if (!user_id)
		return;

	const auto reaction{event.reacting_emoji};
	const auto emoji{reaction.get_mention()};
	if (emoji.empty())
		return;

	const auto usable_emoji{emoji.starts_with("<:") ? emoji.substr(2, emoji.size() - 3) : emoji.substr(1, 1)};
	if (usable_emoji.empty())
		return;

	size_t role_id{Database::get_reaction_role_data(message_id, usable_emoji)};
	if (!role_id)
		return;

	bot.guild_member_add_role(event.reacting_guild->id, event.reacting_member.user_id, role_id);
}

static inline auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) noexcept
	-> void {
	const auto message_id{event.message_id};
	if (!message_id)
		return;

	const auto user_id{event.reacting_user_id};
	if (!user_id)
		return;

	const auto reaction{event.reacting_emoji};
	const auto emoji{reaction.get_mention()};
	if (emoji.empty())
		return;

	const auto usable_emoji{emoji.starts_with("<:") ? emoji.substr(2, emoji.size() - 3) : emoji.substr(1, 1)};
	if (usable_emoji.empty())
		return;

	size_t role_id{Database::get_reaction_role_data(message_id, usable_emoji)};
	if (!role_id)
		return;

	bot.guild_member_remove_role(event.reacting_guild->id, event.reacting_user_id, role_id);
}
}  // namespace roles