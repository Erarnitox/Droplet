#include "ChallengeBadgeCommand.hpp"
#include <appcommand.h>
#include <colors.h>

#include <Core.hpp>
#include <variant>

#include "ChallengeRoleDTO.hpp"
#include "ChallengeRoleRepository.hpp"
#include "IButtonCommand.hpp"
#include "IFormCommand.hpp"
#include "IGlobalSlashCommand.hpp"

ChallengeBadgeCommand::ChallengeBadgeCommand() : IGlobalSlashCommand(), IButtonCommand(), IFormCommand() {
	this->command_name = "challenge_badge";
	this->command_description = "Create challenge Badge (Admin only!)";

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_channel, "channel", "In which channel to post the challenge in", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "question", "What is the question that needs to be solved?", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "solution", "The solution that needs to be entered", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "badge", "The badge that will be granted", true));
	
	this->command_options.emplace_back(
		dpp::command_option(dpp::co_integer, "xp", "The amount of xp that will be granted", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "title", "The title for the challenge", true));
}

void ChallengeBadgeCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		event.reply(dpp::message("Only admins are allowed to use this command!").set_flags(dpp::m_ephemeral));
		return;
	}

	const auto channel{Core::get_parameter(*Bot::ctx, event, "channel")};
	if (channel.empty()) {
		return;
	}

	const auto question{Core::get_parameter(*Bot::ctx, event, "question")};
	if (question.empty()) {
		return;
	}

	const auto solution{Core::get_parameter(*Bot::ctx, event, "solution")};
	if (solution.empty()) {
		return;
	}

	const auto badge{Core::get_parameter(*Bot::ctx, event, "badge")};
	if (badge.empty()) {
		return;
	}

	const auto xp{Core::get_parameter(*Bot::ctx, event, "xp")};
	if (xp.empty()) {
		return;
	}

	const auto title{Core::get_parameter(*Bot::ctx, event, "title")};
	if (title.empty()) {
		return;
	}

	const auto channel_id{Core::get_channel_id(channel)};
	if (channel_id.empty()) {
		event.reply(dpp::message("No valid Channel provided!").set_flags(dpp::m_ephemeral));
		return;
	}

	const auto guild_id{event.command.guild_id};
	if (!guild_id) {
		event.reply(dpp::message("Something went wrong...").set_flags(dpp::m_ephemeral));
		return;
	}

	// create the challenge message
	dpp::embed embed = dpp::embed()
						   .set_color(dpp::colors::green_apple)
						   .set_title(title)
						   .add_field("Challenge:", question, true)
						   .add_field("Badge Reward:", badge, false)
						   .add_field("EXP Reward:", xp, false);

	dpp::message msg(channel_id, embed);

	msg.add_component(dpp::component().add_component(dpp::component()
														 .set_label("Enter Solution")
														 .set_type(dpp::cot_button)
														 .set_emoji("🚩")
														 .set_style(dpp::cos_success)
														 .set_id("solve_challenge_badge_btn")));

	// send the challenge message
	Bot::ctx->message_create(
		msg, [role_id, badge, xp, event, question, solution, guild_id](const dpp::confirmation_callback_t& cb) -> void {
			auto sent_message{cb.value};

			size_t message_id{0};
			try {
				message_id = std::get<dpp::message>(sent_message).id;
				if (message_id == 0) {
					event.reply(dpp::message("Something went wrong! No message created!").set_flags(dpp::m_ephemeral));
					return;
				}
			} catch (...) {
				event.reply(dpp::message("Could not get created message! ...").set_flags(dpp::m_ephemeral));
				return;
			}

			// save the needed information in the database
			ChallengeBadgeRepository repo;
			ChallengeBadgeDTO data{badge, xp, guild_id, message_id, solution};
			if (repo.create(data)) {
				Bot::ctx->log(dpp::ll_info,
							  std::format("Challenge badge with message_id={} was "
										  "inserted into the Databse",
										  message_id));
			} else {
				event.reply(dpp::message("Could not save Challenge Data to Database! ...").set_flags(dpp::m_ephemeral));
				Bot::ctx->log(dpp::ll_error,
							  std::format("Challenge Badge Data could not be saved to "
										  "Database! (message_id={})",
										  message_id));
				Bot::ctx->message_delete(message_id, std::get<dpp::message>(sent_message).channel_id);
				return;
			}

			// send a confirmation to the admin
			event.reply(dpp::message(std::format("Challenge Created!\nQuestion: {}\nReward: {} - EXP:{}", question, badge, xp))
							.set_flags(dpp::m_ephemeral));
		});

	return;
}

void ChallengeBadgeCommand::on_button_click(const dpp::button_click_t& event) {
	if (event.custom_id != "solve_challenge_badge_btn") {
		return;
	}

	/* Instantiate an interaction_modal_response object */
	dpp::interaction_modal_response modal("challenge_badge_solution", "Please enter the correct Solution!");

	/* Add a text component */
	modal.add_component(dpp::component()
							.set_label("Solution:")
							.set_id("solution_id")
							.set_type(dpp::cot_text)
							.set_placeholder("Answer")
							.set_min_length(1)
							.set_max_length(128)
							.set_text_style(dpp::text_short));

	/* Trigger the dialog box. All dialog boxes are ephemeral */
	event.dialog(modal);

	return;
}

void ChallengeBadgeCommand::on_form_submit(const dpp::form_submit_t& event) {
	if (event.custom_id != "challenge_badge_solution") {
		return;
	}

	// get the needed data from the event
	const auto msg_id{event.command.message_id};
	const auto member{event.command.member};

	// get the correct answer and reward role from the database
	ChallengeBadgeRepository repo;
	ChallengeBadgeDTO dto = repo.get(msg_id);

	if (!dto.badge || dto.solution.size() == 0) {
		Bot::ctx->log(dpp::ll_warning,
					  std::format("Got invalid data from Database in "
								  "ChallengeBadgeCommand::handleFormSubmits.\nData: "
								  "badge={}, dto.solution={}",
								  dto.badge,
								  dto.solution));
		event.reply(dpp::message("OOPS! Something went wrong! Please contact "
								 "@erarnitox with this error code: 298374")
						.set_flags(dpp::m_ephemeral));
		return;
	}

	const auto entered_variant{event.components[0].components[0].value};
	const auto entered_ptr{std::get_if<std::string>(&entered_variant)};
	if (!entered_ptr) {
		Bot::ctx->log(dpp::ll_warning, "Corrupted Data occured in ChallengeBadgeCommand::handleFormSubmits");
		event.reply(dpp::message("OOPS! Something went wrong! Please contact "
								 "@erarnitox with this error code: 298375")
						.set_flags(dpp::m_ephemeral));
		return;
	}

	const auto& entered{*entered_ptr};

	if (entered == dto.solution) {
		//Bot::ctx->guild_member_add_role(event.command.guild_id, member.user_id, dto.roleId);

		event.reply(dpp::message(std::format("Well done {}, you solved this challenge!", member.get_mention()))
						.set_flags(dpp::m_ephemeral));
	} else {
		event.reply(dpp::message(std::format("Sorry {}, this is not the right answer!", member.get_mention()))
						.set_flags(dpp::m_ephemeral));
	}
}