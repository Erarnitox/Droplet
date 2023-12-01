#include "MemberCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <fmt/core.h>
#include <snowflake.h>

#include <Core.hpp>
#include <MemberRepository.hpp>
#include <variant>

MemberCommand::MemberCommand() : IGlobalSlashCommand() {
	this->command_name = "member";
	this->command_description = "Manage dropsoft team members (Admin only!)";

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "action", "what would you like to do?", true)
			.add_choice(dpp::command_option_choice("Add a team Member", std::string("member_add")))
			.add_choice(dpp::command_option_choice("Remove a team Member", std::string("member_remove"))));

	this->command_options.emplace_back(dpp::command_option(dpp::co_user, "member", "The Member to manage", true));
}

void MemberCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (event.command.guild_id != dpp::snowflake(808151108748836914ull)){
		event.reply("This command can only be executed on the DropSoft Server!");
		return;
	}

	const auto& cmd{event.command};

	const auto action{Core::get_parameter(*Bot::ctx, event, "action")};
	const auto member{cmd.get_resolved_user(std::get<dpp::snowflake>(event.get_parameter("member")))};

	MemberRepository repo;
	MemberDTO data{member.id, member.get_avatar_url(), member.username};

	if (action == "member_add") {
		if (repo.create(data)) {
			auto msg{dpp::message("Member added!")};
			event.reply(msg);
		} else {
			event.reply(dpp::message("Error: Member can't be saved to the database!").set_flags(dpp::m_ephemeral));
		}
		return;
	} else if (action == "member_remove") {
		if (repo.remove(data.discord_id)) {
			auto msg{dpp::message("Member removed!")};
			event.reply(msg);
		} else {
			event.reply(dpp::message("Error: Member can't be deleted from the database!").set_flags(dpp::m_ephemeral));
		}
		return;
	}

	event.reply(dpp::message("Error: Something went wrong!").set_flags(dpp::m_ephemeral));
	return;
}