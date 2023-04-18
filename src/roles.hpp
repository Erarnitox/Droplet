#pragma once

#include "core.hpp"
#include <dpp/colors.h>
#include <dpp/message.h>
#include <fmt/core.h>

namespace roles {
    static
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        dpp::slashcommand challenge_role("challenge_role", "Create challenge Roles (Admin only!)", bot.me.id);
        
        /*
        challenge_role.add_option(
	        dpp::command_option(dpp::co_string, "action", "What action would you like to perform?", true)
	            .add_choice(dpp::command_option_choice("Create a new challenge role", std::string("action_create")))
	            //.add_choice(dpp::command_option_choice("Update an existing challenge role", std::string("action_update")))
                //.add_choice(dpp::command_option_choice("Delete an existing challenge role", std::string("action_delete")))
        );*/

        challenge_role.add_option(
	        dpp::command_option(dpp::co_string, "channel", "In which channel to post the challenge in", true)
        );

        challenge_role.add_option(
	        dpp::command_option(dpp::co_string, "question", "What is the question that needs to be solved?", true)
        );

        challenge_role.add_option(
	        dpp::command_option(dpp::co_string, "solution", "The solution that needs to be entered", true)
        );

        challenge_role.add_option(
	        dpp::command_option(dpp::co_string, "role", "The role that will be granted", true)
        );

        challenge_role.add_option(
	        dpp::command_option(dpp::co_string, "title", "The title for the challenge", true)
        );

        dpp::slashcommand reaction_role("reaction_role", "Create reaction Roles (Admin only!)", bot.me.id);

        /*
        reaction_role.add_option(
	        dpp::command_option(dpp::co_string, "action", "What action would you like to perform?", true)
	            .add_choice(dpp::command_option_choice("Create a new reaction role", std::string("action_create")))
	            .add_choice(dpp::command_option_choice("Update an existing reaction role", std::string("action_update")))
                .add_choice(dpp::command_option_choice("Delete an existing reaction role", std::string("action_delete")))
        );*/

        reaction_role.add_option(
	        dpp::command_option(dpp::co_string, "channel", "In which channel to post the message in", true)
        );

        reaction_role.add_option(
	        dpp::command_option(dpp::co_string, "emoji", "The emoji the user needs to react with", true)
        );

        reaction_role.add_option(
	        dpp::command_option(dpp::co_string, "role", "The role that will be granted", true)
        );

        command_list.push_back(challenge_role);
        command_list.push_back(reaction_role);
    }

    static
    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
        if (event.command.get_command_name() == "challenge_role") {
            if(!core::is_admin(event.command.member)){
                core::timed_reply(event, std::string("Only admins are allowed to use this command!"), 2000);
            }
            const auto& channel{ std::get<std::string>(event.get_parameter("channel")) };
            const auto& question{ std::get<std::string>(event.get_parameter("question")) };
            const auto& solution{ std::get<std::string>(event.get_parameter("solution")) };
            const auto& role{ std::get<std::string>(event.get_parameter("role")) };
            const auto& title{ std::get<std::string>(event.get_parameter("title")) };
            const auto& role_id{core::get_role_id(role) };
            const auto& channel_id{ core::get_channel_id(channel) };

            // create the challenge message
            dpp::embed embed = dpp::embed().
	            set_color(dpp::colors::black).
	            set_title(title).
                add_field(
                    "Challenge:",
                    question,
                    true
                ).
                add_field(
                    "Role Reward:", 
                    role,
                    false
                );
            
            dpp::message msg(channel_id, embed);

            msg.add_component(dpp::component().add_component(
	            dpp::component().set_label("Enter Solution").
	            set_type(dpp::cot_button).
	            set_emoji("🚩").
	            set_style(dpp::cos_success).
	            set_id("solve_challenge_btn")
	        ));

            // send the challenge message
            bot.message_create(msg);

            // send a confirmation to the admin
            core::timed_reply(
                event, 
                fmt::format(
                    "Challenge Created!\nQuestion: {}\nReward: {}\nMessage ID: {}",
                    question, role, msg.id
                ), 
                10000 //10sek
            );
        }
        else if (event.command.get_command_name() == "reaction_role") {
            if(!core::is_admin(event.command.member)){
                core::timed_reply(event, std::string("Only admins are allowed to use this command!"), 2000);
            }
            // reaction role logic
        }
    }

    static
    auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void {
        if(event.custom_id != "solve_challenge_btn") return;

	    /* Instantiate an interaction_modal_response object */
	    dpp::interaction_modal_response modal("Enter the Solution", "Please enter the correct Solution!");
	    
        /* Add a text component */
	    modal.add_component(
	        dpp::component().
	        set_label("Solution:").
	        set_id("solution_id").
	        set_type(dpp::cot_text).
	        set_placeholder("Answer").
	        set_min_length(4).
	        set_max_length(64).
	        set_text_style(dpp::text_short)
	    );

	    /* Trigger the dialog box. All dialog boxes are ephemeral */
	    event.dialog(modal);
    }

    static
    auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot) -> void {
        // get the needed data from the event
        const auto& msg_id{ event.command.message_id };
        const auto& member{ event.command.member };

        // get the correct answer and reward role from the database
        auto role_id{ 812457935526821888L }; //TODO
        auto flag{ std::string("correct")}; //TODO

        const auto& entered { std::get<std::string>(event.components[0].components[0].value) };

        if(entered == flag) {
            bot.guild_member_add_role(event.command.guild_id, member.user_id, role_id);

            core::timed_reply(
                event,
                fmt::format("Well done {}, you solved this challenge!", member.get_mention()),
                5000
            );
        } else {
            core::timed_reply(
                event,
                fmt::format("Sorry {}, this is not the right answer!", member.get_mention()),
                5000
            );
        }
    }

    static
    auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void {
        const auto& message_id{ event.message_id };
        const auto& user_id{ event.reacting_user.id };
        const auto& reaction{ event.reacting_emoji };

        //TODO: implement reaction roles
    }

    static
    auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void {
        const auto& message_id{ event.message_id };
        const auto& user_id{ event.reacting_user_id };
        const auto& reaction{ event.reacting_emoji };

        //TODO: implement reaction roles
    }
}