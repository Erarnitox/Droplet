/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class manages all commands and their registration
 *	so the bot knows about it.
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include <Commands.hpp>
#include <memory>

///////////////////////////////////////
/// COMMAND HEADERS:
///////////////////////////////////////

#include <AddResourcesCommand.hpp>
#include <BlacklistCommand.hpp>
#include <ChallengeBadgeCommand.hpp>
#include <ChallengeRoleCommand.hpp>
#include <CleanUp.hpp>
#include <ClearCommand.hpp>
#include <FlipCommand.hpp>
#include <GetResourcesCommand.hpp>
#include <HelpCommand.hpp>
#include <InviteCommand.hpp>
#include <LearnCPPCommand.hpp>
#include <ProfileCommand.hpp>
#include <ReactionRoles.hpp>
#include <ReminderCommand.hpp>
#include <RemoveNotificationCommand.hpp>
#include <RemovePortalCommand.hpp>
#include <RollCommand.hpp>
#include <ServerDetails.hpp>
#include <SetNotificationCommand.hpp>
#include <SetPortalCommand.hpp>
#include <SetStatus.hpp>

/**
 * @brief registers the commands so they are known to the bot
 * @return doesn't return anything
 */
void Commands::registerCommands() {
	////////////////////////////////////
	/// REGISTER ALL COMMANDS HERE:
	////////////////////////////////////
	Bot::add_ready_command(std::make_shared<SetStatus>());
	Bot::add_ready_command(std::make_shared<SetNotificationCommand>());
	Bot::add_message_command(std::make_shared<CleanUp>());
	Bot::add_slash_command(std::make_shared<HelpCommand>());
	Bot::add_slash_command(std::make_shared<ServerDetails>());
	Bot::add_slash_command(std::make_shared<InviteCommand>());
	Bot::add_slash_command(std::make_shared<LearnCPPCommand>());
	Bot::add_slash_command(std::make_shared<AddResourcesCommand>());
	Bot::add_slash_command(std::make_shared<GetResourcesCommand>());
	Bot::add_slash_command(std::make_shared<RollCommand>());
	Bot::add_slash_command(std::make_shared<FlipCommand>());
	Bot::add_slash_command(std::make_shared<ProfileCommand>());
	Bot::add_slash_command(std::make_shared<RemovePortalCommand>());
	Bot::add_slash_command(std::make_shared<BlacklistCommand>());
	Bot::add_slash_command(std::make_shared<SetNotificationCommand>());
	Bot::add_slash_command(std::make_shared<RemoveNotificationCommand>());
	Bot::add_slash_command(std::make_shared<ReminderCommand>());
	Bot::add_slash_command(std::make_shared<ClearCommand>());

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

	const auto& portal_command{std::make_shared<SetPortalCommand>()};
	Bot::add_slash_command(portal_command);
	Bot::add_message_command(portal_command);
	Bot::add_reaction_command(portal_command);
}
