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

#include "ChallengeRoleCommand.hpp"

#include <appcommand.h>
#include <colors.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <variant>

#include "ChallengeRoleDTO.hpp"
#include "ChallengeRoleRepository.hpp"
#include "IButtonCommand.hpp"
#include "IFormCommand.hpp"
#include "IGlobalSlashCommand.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
ChallengeRoleCommand::ChallengeRoleCommand(AppContext& ctx) : discord_(ctx.discord), db_(ctx.db) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->admin_only = true;

	this->command_options.emplace_back(dpp::co_channel, "channel", "In which channel to post the challenge in", true);

	this->command_options.emplace_back(
		dpp::co_string, "question", "What is the question that needs to be solved?", true);

	this->command_options.emplace_back(dpp::co_string, "solution", "The solution that needs to be entered", true);

	this->command_options.emplace_back(dpp::co_role, "role", "The role that will be granted", true);

	this->command_options.emplace_back(dpp::co_string, "title", "The title for the challenge", true);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void ChallengeRoleCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != "challenge_role") {
		return;
	}

	if (not Core::is_admin(event.command.member)) {
		event.reply(dpp::message("Only admins are allowed to use this command!").set_flags(dpp::m_ephemeral));
		return;
	}

	const auto channel_id{std::get<dpp::snowflake>(event.get_parameter("channel"))};

	const auto question{Core::get_parameter(discord_, event, "question")};
	if (question.empty()) {
		return;
	}

	const auto solution{Core::get_parameter(discord_, event, "solution")};
	if (solution.empty()) {
		return;
	}

	const auto role_id{std::get<dpp::snowflake>(event.get_parameter("role"))};
	const auto role{event.command.get_resolved_role(role_id)};

	const auto title{Core::get_parameter(discord_, event, "title")};
	if (title.empty()) {
		return;
	}

	if (role_id.empty()) {
		event.reply(dpp::message("No valid Role provided!").set_flags(dpp::m_ephemeral));
		return;
	}

	if (channel_id.empty()) {
		event.reply(dpp::message("No valid Channel provided!").set_flags(dpp::m_ephemeral));
		return;
	}

	const auto guild_id{event.command.guild_id};
	if (not guild_id) {
		event.reply(dpp::message("Something went wrong...").set_flags(dpp::m_ephemeral));
		return;
	}

	// create the challenge message
	const dpp::embed embed = dpp::embed()
								 .set_color(dpp::colors::black)
								 .set_title(title)
								 .add_field("Challenge:", question, true)
								 .add_field("Role Reward:", role.get_mention(), false);

	dpp::message msg(channel_id, embed);

	msg.add_component(dpp::component().add_component(dpp::component()
														 .set_label("Enter Solution")
														 .set_type(dpp::cot_button)
														 .set_emoji("🚩")
														 .set_style(dpp::cos_success)
														 .set_id("solve_challenge_btn")));

	// send the challenge message
	discord_.message_create(
		msg,
		[this, role_id, role, event, question, solution, guild_id](const dpp::confirmation_callback_t& cb) -> void {
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
			ChallengeRoleRepository repo{db_};
			const ChallengeRoleDTO data{role_id, guild_id, message_id, solution};
			if (repo.create(data)) {
				discord_.log(dpp::ll_info,
							 std::format("Challenge role with message_id={} was "
										 "inserted into the Databse",
										 message_id));
			} else {
				event.reply(dpp::message("Could not save Challenge Data to Database! ...").set_flags(dpp::m_ephemeral));
				discord_.log(dpp::ll_error,
							 std::format("Challenge Role Data could not be saved to "
										 "Database! (message_id={})",
										 message_id));
				discord_.message_delete(message_id, std::get<dpp::message>(sent_message).channel_id);
				return;
			}

			// send a confirmation to the admin
			event.reply(
				dpp::message(std::format("Challenge Created!\nQuestion: {}\nReward: {}", question, role.get_mention()))
					.set_flags(dpp::m_ephemeral));
		});

	return;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void ChallengeRoleCommand::on_button_click(const dpp::button_click_t& event) {
	if (event.custom_id != "solve_challenge_btn") {
		return;
	}

	/* Instantiate an interaction_modal_response object */
	dpp::interaction_modal_response modal("challenge_role_solution", "Please enter the correct Solution!");

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

//-----------------------------------------------------
//
//-----------------------------------------------------
void ChallengeRoleCommand::on_form_submit(const dpp::form_submit_t& event)
{
    if (event.custom_id != "challenge_role_solution") {
        return;
    }

    const auto msg_id = event.command.message_id;
    const auto& member = event.command.member;
    const auto guild_id = event.command.guild_id;

    if (!msg_id || !member.user_id || !guild_id) {
        event.reply(
            dpp::message("Can't acquire the required data. Please try again.")
                .set_flags(dpp::m_ephemeral));
        return;
    }

    const auto entered_opt = Core::find_text_input(event.components);

    if (!entered_opt.has_value()) {
        discord_.log(
            dpp::ll_warning,
            "ChallengeRoleCommand: modal input has an unexpected value type.");

        event.reply(
            dpp::message("Invalid form submission. Please try again.")
                .set_flags(dpp::m_ephemeral));
        return;
    }

    const std::string entered = *entered_opt;

    ChallengeRoleRepository repo{db_};
    const ChallengeRoleDTO dto = repo.get(msg_id);

    if (!dto.roleId || dto.solution.empty()) {
        discord_.log(dpp::ll_warning, "ChallengeRoleCommand: missing or invalid challenge data");

        event.reply(
            dpp::message(
                "Something went wrong. Please contact @erarnitox "
                "with error code 298364.")
                .set_flags(dpp::m_ephemeral));
        return;
    }

    if (entered != dto.solution) {
        event.reply(
            dpp::message(
                std::format(
                    "Sorry {}, this is not the right answer!",
                    member.get_mention()))
                .set_flags(dpp::m_ephemeral));
        return;
    }

    // Do not claim success until Discord confirms the role assignment.
    discord_.guild_member_add_role(
        guild_id,
        member.user_id,
        dto.roleId,
        [this, event, member](const dpp::confirmation_callback_t& cb) {
            if (cb.is_error()) {
                discord_.log(
                    dpp::ll_error,
                    std::format(
                        "ChallengeRoleCommand: failed to grant challenge role: {}",
                        cb.get_error().message));

                event.reply(
                    dpp::message(
                        "Your answer was correct, but the role could not "
                        "be granted. Please contact an administrator.")
                        .set_flags(dpp::m_ephemeral));
                return;
            }

            event.reply(
                dpp::message(
                    std::format(
                        "Well done {}, you solved this challenge and received "
                        "the role!",
                        member.get_mention()))
                    .set_flags(dpp::m_ephemeral));
        });
}