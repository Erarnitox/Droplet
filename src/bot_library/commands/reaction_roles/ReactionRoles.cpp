#include "ReactionRoles.hpp"

#include <appcommand.h>

#include <cstdint>
#include <format>
#include <variant>

#include "Core.hpp"
#include "IReactionCommand.hpp"
#include "ReactionRoleDTO.hpp"
#include "ReactionRoleRepository.hpp"

ReactionRoles::ReactionRoles() : IGlobalSlashCommand(), IReactionCommand() {
	this->command_name = "reaction_role";
	this->command_description = "Create reaction Roles (Admin only!)";

	this->command_options.emplace_back(dpp::co_string, "message_link", "A link to the message to react to", true);

	this->command_options.emplace_back(dpp::co_string, "emoji", "The emoji the user needs to react with", true);

	this->command_options.emplace_back(dpp::co_role, "role", "The role that will be granted", true);
}

void ReactionRoles::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		event.reply(dpp::message("Only admins are allowed to use this command!").set_flags(dpp::m_ephemeral));
		return;
	}

	const auto message_link{Core::get_parameter(*Bot::ctx, event, "message_link")};
	if (message_link.empty())
		return;

	const auto emoji{Core::get_parameter(*Bot::ctx, event, "emoji")};
	if (emoji.empty())
		return;

	const auto role_id{std::get<dpp::snowflake>(event.get_parameter("member"))};
	
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
	ReactionRoleRepository repo;

	const size_t i_role_id{static_cast<size_t>(role_id)};
	const size_t i_message_id{static_cast<size_t>(std::stoll(message_id))};
	const size_t& guild_id{event.command.guild_id};

	ReactionRoleDTO dto{i_role_id, i_message_id, guild_id, usable_emoji};

	if (repo.create(dto)) {
		// Let the bot react to the message
		Bot::ctx->message_add_reaction(message_id, channel_id, usable_emoji);

		// send a confirmation to the admin
		event.reply(
			dpp::message(
				std::format("Reaction Role Created!\nMessage: {}\nReaction: {}\nRole: <@{}>", message_link, emoji, i_role_id))
				.set_flags(dpp::m_ephemeral));
	} else {
		event.reply(dpp::message("Reaction Role can't be saved to Database!").set_flags(dpp::m_ephemeral));
	}

	return;
}

void ReactionRoles::on_message_reaction_add(const dpp::message_reaction_add_t& event) {
	const auto message_id{event.message_id};
	if (!message_id) {
		return;
	}

	const auto user_id{event.reacting_user.id};
	if (!user_id) {
		return;
	}

	const auto reaction{event.reacting_emoji};
	const auto emoji{reaction.get_mention()};
	if (emoji.empty()) {
		return;
	}

	const auto usable_emoji{Core::simple_hash(emoji)};

	ReactionRoleRepository repo;
	ReactionRoleDTO dto{repo.get(message_id, usable_emoji)};

	if (!dto.role_id) {
		return;
	}

	Bot::ctx->guild_member_add_role(event.reacting_guild->id, event.reacting_member.user_id, dto.role_id);

	return;
}

void ReactionRoles::on_message_reaction_remove(const dpp::message_reaction_remove_t& event) {
	const auto message_id{event.message_id};
	if (!message_id) {
		return;
	}

	const auto user_id{event.reacting_user_id};
	if (!user_id) {
		return;
	}

	const auto reaction{event.reacting_emoji};
	const auto emoji{reaction.get_mention()};
	if (emoji.empty()) {
		return;
	}

	const auto usable_emoji{Core::simple_hash(emoji)};

	ReactionRoleRepository repo;
	ReactionRoleDTO dto{repo.get(message_id, usable_emoji)};

	if (!dto.role_id) {
		return;
	}

	Bot::ctx->guild_member_remove_role(event.reacting_guild->id, event.reacting_user_id, dto.role_id);

	return;
}