#include "ProfileCommand.hpp"

#include <colors.h>
#include <dispatcher.h>
#include <invite.h>
#include <message.h>
#include <misc-enum.h>

#include <ChallengeBadgeDTO.hpp>
#include <ChallengeBadgeRepository.hpp>
#include <HasBadgeRepository.hpp>
#include <UserDTO.hpp>
#include <UserRepository.hpp>
#include <format>
#include <string>
#include <vector>

ProfileCommand::ProfileCommand() : IGlobalSlashCommand() {
	this->command_name = "profile";
	this->command_description = "View Profile Information";
	this->command_options.emplace_back(dpp::command_option(dpp::co_user, "user", "User to view the Profile of", true));
}

void ProfileCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	const auto user_id{std::get<dpp::snowflake>(event.get_parameter("user"))};
	const auto member{event.command.get_resolved_member(user_id)};

	// Get the user from the usr Table
	UserRepository user_repo;
	UserDTO user_dto;

	try {
		user_dto = user_repo.get(static_cast<size_t>(user_id));
	} catch (...) {
		Bot::ctx->log(dpp::ll_warning, "User is not in usr database yet");
	}

	std::map<std::string, std::vector<std::string>> badges;

	ChallengeBadgeRepository badge_repo;
	HasBadgeRepository has_badge_repo;

	if (user_dto.user_id) {
		// Get all badges a the user has earned:
		for (const auto badge_id : has_badge_repo.get(static_cast<size_t>(user_id))) {
			ChallengeBadgeDTO badge_dto{badge_repo.get(badge_id)};

			try {
				badges.at(badge_dto.guild_name).push_back(badge_dto.badge);
			} catch (...) {
				badges[badge_dto.guild_name] = std::vector<std::string>();
				badges[badge_dto.guild_name].push_back(badge_dto.badge);
			}
		}
	}

	/* create the embed */
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::blue_eyes)
						 .set_title(std::format("{}", user_dto.user_name))
						 .set_description(user_dto.user_id ? "Badges" : "")
						 .set_thumbnail(member.get_avatar_url())};

	// Add all badge fields:
	for (const auto& guild : badges) {
		std::string badge_string;

		for (const auto& badge : guild.second) {
			badge_string.append(badge);
		}

		embed.add_field(guild.first, badge_string);
	}

	// Level:
	const auto& exp{user_dto.exp};
	const auto level{exp / 500};
	const auto missing{500 - ((exp + 501) % 500)};

	embed.add_field(std::format("LEVEL: [{}]", level),
					std::format("{}EXP/{}EXP to Reach Level {}", exp, (exp + missing), (level + 1)));

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	return;
}