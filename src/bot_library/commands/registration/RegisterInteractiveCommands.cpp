#include <Bot.hpp>
#include <BugReportCommand.hpp>
#include <ChallengeBadgeCommand.hpp>
#include <ChallengeRoleCommand.hpp>
#include <ReactionRoles.hpp>
#include <memory>

#include "RegisterModules.hpp"

void register_interactive_commands(Bot& bot, AppContext& ctx) {
	const auto bug_report_command{std::make_shared<BugReportCommand>(ctx)};
	bot.add_slash_command(bug_report_command);
	bot.add_form_command(bug_report_command);

	const auto challenge_command{std::make_shared<ChallengeRoleCommand>(ctx)};
	bot.add_slash_command(challenge_command);
	bot.add_button_command(challenge_command);
	bot.add_form_command(challenge_command);

	const auto badge_command{std::make_shared<ChallengeBadgeCommand>(ctx)};
	bot.add_slash_command(badge_command);
	bot.add_button_command(badge_command);
	bot.add_form_command(badge_command);

	const auto reaction_roles{std::make_shared<ReactionRoles>(ctx)};
	bot.add_slash_command(reaction_roles);
	bot.add_reaction_command(reaction_roles);
}
