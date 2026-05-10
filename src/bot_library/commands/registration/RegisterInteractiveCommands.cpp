/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include <Bot.hpp>
#include <BugReportCommand.hpp>
#include <ChallengeBadgeCommand.hpp>
#include <ChallengeRoleCommand.hpp>
#include <ReactionRoles.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_interactive_commands() {
	const auto& bug_report_command{std::make_shared<BugReportCommand>()};
	Bot::add_slash_command(bug_report_command);
	Bot::add_form_command(bug_report_command);

	const auto& challenge_command{std::make_shared<ChallengeRoleCommand>()};
	Bot::add_slash_command(challenge_command);
	Bot::add_button_command(challenge_command);
	Bot::add_form_command(challenge_command);

	const auto& badge_command{std::make_shared<ChallengeBadgeCommand>()};
	Bot::add_slash_command(badge_command);
	Bot::add_button_command(badge_command);
	Bot::add_form_command(badge_command);

	const auto& reaction_roles{std::make_shared<ReactionRoles>()};
	Bot::add_slash_command(reaction_roles);
	Bot::add_reaction_command(reaction_roles);
}
