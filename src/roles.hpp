#pragma once

#include <core.hpp>
#include "database.hpp"
#include <dpp/colors.h>
#include <dpp/message.h>
#include <dpp/restresults.h>
#include <fmt/core.h>
#include <stdexcept>
#include <string>
#include <stack>

namespace roles {
    static inline
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) noexcept -> void {
        // Challenge Roles
        dpp::slashcommand challenge_role("challenge_role", "Create challenge Roles (Admin only!)", bot.me.id);
        
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

        // Reaction Role
        dpp::slashcommand reaction_role("reaction_role", "Create reaction Roles (Admin only!)", bot.me.id);

        reaction_role.add_option(
	        dpp::command_option(dpp::co_string, "message_link", "A link to the message to react to", true)
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

    static inline
    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) noexcept -> void {
        if (event.command.get_command_name() == "challenge_role") {
            if(!core::is_admin(event.command.member)){
                core::timed_reply(event, std::string("Only admins are allowed to use this command!"), 2000);
                return;
            }

            const auto channel{ core::get_parameter(event, "channel") };
            if(channel.empty()) return;
            
            const auto question{ core::get_parameter(event, "question") };
            if(question.empty()) return;

            const auto solution{ core::get_parameter(event, "solution") };
            if(solution.empty()) return;
            
            const auto role{ core::get_parameter(event, "role") };
            if(role.empty()) return;

            const auto title{ core::get_parameter(event, "title") };
            if(title.empty()) return;

            const auto role_id{ core::get_role_id(role) };
            if(role_id.empty()) {
                core::timed_reply(event, "No valid Role provided!", 2000);
                return;
            }

            const auto channel_id{ core::get_channel_id(channel) };
            if(channel_id.empty()) {
                core::timed_reply(event, "No valid Channel provided!", 2000);
                return;
            }

            const auto guild_id{ event.command.guild_id };
            if(!guild_id) {
                core::timed_reply(event, "Something went wrong...", 2000);
                return;
            }

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
            bot.message_create(
                msg,
                [role_id, role, event, question, solution, guild_id](const dpp::confirmation_callback_t& cb) -> void {
                    auto sent_message{ cb.value };

                    size_t sane_role_id;
                    try {
                        sane_role_id = std::stoul(role_id); 
                    } catch(std::invalid_argument exception){
                        core::timed_reply(event, "Bad role! Just mention the role!", 5000);
                        return;
                    }
                    
                    size_t message_id{ 0 };
                    try{ 
                        message_id = std::get<dpp::message>(sent_message).id;
                       if(message_id == 0){
                        core::timed_reply(event, "Something went wrong! No message created! ...", 5000);
                        return;
                       } 
                    } catch(...){
                        core::timed_reply(event, "Could not get created message! ...", 5000);
                        return;
                    }

                    // save the needed information in the database
                    Database::insert_challenge_role_data(
                        sane_role_id,
                        guild_id, 
                        message_id,
                        solution
                    );

                    // send a confirmation to the admin
                    core::timed_reply(
                        event, 
                        fmt::format(
                            "Challenge Created!\nQuestion: {}\nReward: {}",
                            question, role
                        ), 
                        10000 //10sek
                    );
                    
                }
            );
        }
        else if (event.command.get_command_name() == "reaction_role") {
            if(!core::is_admin(event.command.member)){
                core::timed_reply(event, std::string("Only admins are allowed to use this command!"), 2000);
                return;
            }

            const auto message_link{ core::get_parameter(event, "message_link") };
            if(message_link.empty()) return;

            const auto emoji{ core::get_parameter(event, "emoji") };
            if(emoji.empty()) return;
            
            const auto role{ core::get_parameter(event, "role") };
            if(role.empty()) return;

            const auto role_id{ core::get_role_id(role) };
            if(role_id.empty()) {
                core::timed_reply(event, "No valid Role was provided!", 2000);
                return;
            }

            const auto usable_emoji{emoji.starts_with("<:") ? emoji.substr(2, emoji.size()-3) : emoji };
            if(usable_emoji.empty()) {
                core::timed_reply(event, "No valid emoji was provided!", 2000);
                return;
            }
            
            // indecies of slashes in the link
            std::vector<size_t> slashes;

            if(message_link.size() < 4 || !message_link.starts_with("http")) {
                event.reply("The provided link is not a valid message link!");
                return;
            }

            // find the position of all slashes
           for(size_t i{ 0 };;) {
                i = message_link.find("/", i+1);
                if(i == std::string::npos) break;
                slashes.push_back(i);
            }
            
            if(slashes.size() < 3){
                event.reply("The provided link is not a valid message link!");
                return;
            }

            //--------------------------------------------------
            // get message id from the link
            size_t end_pos{ message_link.size() };
            size_t start_pos{ slashes.back()+1 };
            const auto& message_id{ message_link.substr(start_pos, end_pos) };
            //--------------------------------------------------

            //--------------------------------------------------
            // get message id from the link
            slashes.pop_back();
            end_pos = (start_pos-2) - slashes.back();
            start_pos = slashes.back()+1;
            const auto& channel_id{ message_link.substr(start_pos, end_pos) };
            //--------------------------------------------------

            // Insert Reaction Role into Database
            Database::insert_reaction_role_data(role_id, event.command.guild_id, message_id, usable_emoji);

            // Let the bot react to the message
            bot.message_add_reaction(message_id, channel_id, usable_emoji);

            // send a confirmation to the admin
            core::timed_reply(
                event, 
                fmt::format(
                    "Reaction Role Created!\nMessage: {}\nReaction: {}\nRole: {}",
                    message_link, emoji, role
                ), 
                10000 //10sek
            );
        }
    }

    static inline
    auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) noexcept -> void {
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

    static inline
    auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot) noexcept -> void {
        // get the needed data from the event
        const auto msg_id{ event.command.message_id };
        const auto member{ event.command.member };

        // get the correct answer and reward role from the database
        auto [role_id, flag] = Database::get_challenge_role_data(msg_id);

        if(!role_id || flag.size() == 0){
           return; 
        }

        const auto entered_variant{ event.components[0].components[0].value };
        const auto entered_ptr { std::get_if<std::string>(&entered_variant) };
        if(!entered_ptr) return;
        
        const auto& entered{ *entered_ptr };

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

    static inline
    auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) noexcept -> void {
        const auto message_id{ event.message_id };
        if(!message_id) return;

        const auto user_id{ event.reacting_user.id };
        if(!user_id) return;
        
        const auto reaction{ event.reacting_emoji };
        const auto emoji { reaction.get_mention() };
        if(emoji.empty()) return;
        
        const auto usable_emoji{emoji.starts_with("<:") ? emoji.substr(2, emoji.size()-3) : emoji.substr(1,1) };
        if(usable_emoji.empty()) return;
        
        size_t role_id { Database::get_reaction_role_data(message_id, usable_emoji) };
        if(!role_id) return;

        bot.guild_member_add_role(event.reacting_guild->id, event.reacting_member.user_id, role_id);
    }

    static inline
    auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) noexcept -> void {
        const auto message_id{ event.message_id };
        if(!message_id) return;
        
        const auto user_id{ event.reacting_user_id };
        if(!user_id) return;

        const auto reaction{ event.reacting_emoji };
        const auto emoji { reaction.get_mention() };
        if(emoji.empty()) return;
        
        const auto usable_emoji{emoji.starts_with("<:") ? emoji.substr(2, emoji.size()-3) : emoji.substr(1,1) };
        if(usable_emoji.empty()) return;
        
        size_t role_id { Database::get_reaction_role_data(message_id, usable_emoji) };
        if(!role_id) return;

        bot.guild_member_remove_role(event.reacting_guild->id, event.reacting_user_id, role_id);
    }
}