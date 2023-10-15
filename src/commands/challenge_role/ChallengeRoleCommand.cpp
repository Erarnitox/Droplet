#include "ChallengeRoleCommand.hpp"
#include "ChallengeRoleRepository.hpp"
#include "ChallengeRoleDTO.hpp"

#include <Core.hpp>
#include <Commands.hpp>

#include <dpp/misc-enum.h>
#include <fmt/core.h>
#include <variant>

auto ChallengeRoleCommand::registerGlobalSlashCommand(std::vector<dpp::slashcommand> &command_list, const dpp::cluster &bot) noexcept -> void {
    // Challenge Roles
    dpp::slashcommand challenge_role("challenge_role", "Create challenge Roles (Admin only!)", bot.me.id);

    challenge_role.add_option(dpp::command_option(dpp::co_string, "channel", "In which channel to post the challenge in", true));
    challenge_role.add_option(dpp::command_option(dpp::co_string, "question", "What is the question that needs to be solved?", true));
    challenge_role.add_option(dpp::command_option(dpp::co_string, "solution", "The solution that needs to be entered", true));
    challenge_role.add_option(dpp::command_option(dpp::co_string, "role", "The role that will be granted", true));
    challenge_role.add_option(dpp::command_option(dpp::co_string, "title", "The title for the challenge", true));

    command_list.push_back(challenge_role);
}

auto ChallengeRoleCommand::handleGlobalSlashCommand(const dpp::slashcommand_t& event, dpp::cluster& bot, const std::vector<dpp::slashcommand>& command_list) noexcept -> void {
    // unneeded arguments:
    (void) command_list;

    if (event.command.get_command_name() != "challenge_role") return;
    
    if(!Core::isAdmin(event.command.member)){
        event.reply(dpp::message("Only admins are allowed to use this command!").set_flags(dpp::m_ephemeral));
        return;
    }

    const auto channel{ Core::getParameter(bot, event, "channel") };
    if(channel.empty()) return;
            
    const auto question{ Core::getParameter(bot, event, "question") };
    if(question.empty()) return;

    const auto solution{ Core::getParameter(bot, event, "solution") };
    if(solution.empty()) return;
            
    const auto role{ Core::getParameter(bot, event, "role") };
    if(role.empty()) return;

    const auto title{ Core::getParameter(bot, event, "title") };
    if(title.empty()) return;

    const auto role_id{ Core::getRoleId(role) };
    if(role_id.empty()) {
        event.reply(dpp::message("No valid Role provided!").set_flags(dpp::m_ephemeral));
        return;
    }

    const auto channel_id{ Core::getChannelId(channel) };
    if(channel_id.empty()) {
        event.reply(dpp::message("No valid Channel provided!").set_flags(dpp::m_ephemeral));
        return;
    }

    const auto guild_id{ event.command.guild_id };
    if(!guild_id) {
        event.reply(dpp::message("Something went wrong...").set_flags(dpp::m_ephemeral));
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
        [&bot, role_id, role, event, question, solution, guild_id](const dpp::confirmation_callback_t& cb) -> void {
            auto sent_message{ cb.value };

            size_t sane_role_id;
            try {
                sane_role_id = std::stoul(role_id); 
            } catch(std::invalid_argument exception){
                event.reply(dpp::message("Bad role! Just mention the role!").set_flags(dpp::m_ephemeral));
                return;
            }
                    
            size_t message_id{ 0 };
            try{ 
                message_id = std::get<dpp::message>(sent_message).id;
                if(message_id == 0){
                    event.reply(dpp::message("Something went wrong! No message created!").set_flags(dpp::m_ephemeral));
                    return;
                } 
            } catch(...){
                event.reply(dpp::message("Could not get created message! ...").set_flags(dpp::m_ephemeral));
                return;
            }

            // save the needed information in the database
            ChallengeRoleRepository repo;
            ChallengeRoleDTO data { sane_role_id, guild_id, message_id, solution };
            if(repo.create(data)){
                bot.log(dpp::ll_info, fmt::format("Challenge role with message_id={} was inserted into the Databse", message_id));
            } else {
                event.reply(dpp::message("Could not save Challenge Datea to Database! ...").set_flags(dpp::m_ephemeral));
                bot.log(dpp::ll_error, fmt::format("Challenge Role Data could not be saved to Database! (message_id={})", message_id));
                bot.message_delete(message_id, std::get<dpp::message>(sent_message).channel_id);
            }

            // send a confirmation to the admin
            event.reply(dpp::message(
                fmt::format(
                    "Challenge Created!\nQuestion: {}\nReward: {}",
                    question,
                    role    
                )).set_flags(dpp::m_ephemeral));
        }   
    );
}

auto ChallengeRoleCommand::handleButtonClicks(const dpp::button_click_t& event, dpp::cluster& bot) noexcept -> void {
    if(event.custom_id != "solve_challenge_btn") return;

    // unused parameter "bot"
    (void)bot;
    
    /* Instantiate an interaction_modal_response object */
    dpp::interaction_modal_response modal("challenge_role_solution", "Please enter the correct Solution!");
    
    /* Add a text component */
    modal.add_component(
        dpp::component().
        set_label("Solution:").
        set_id("solution_id").
        set_type(dpp::cot_text).
        set_placeholder("Answer").
        set_min_length(1).
        set_max_length(128).
        set_text_style(dpp::text_short)
    );

    /* Trigger the dialog box. All dialog boxes are ephemeral */
    event.dialog(modal);
}

auto ChallengeRoleCommand::handleFormSubmits(const dpp::form_submit_t& event, dpp::cluster& bot) noexcept -> void {
    if(event.custom_id != "challenge_role_solution") return;
    
    // get the needed data from the event
    const auto msg_id{ event.command.message_id };
    const auto member{ event.command.member };
   
    // get the correct answer and reward role from the database
    ChallengeRoleRepository repo;
    ChallengeRoleDTO dto = repo.get(msg_id);

    if(!dto.roleId || dto.solution.size() == 0){
        bot.log(
            dpp::ll_warning, 
            fmt::format(
                "Got invalid data from Database in ChallengeRoleCommand::handleFormSubmits.\nData: roleId={}, dto.solution={}",
                dto.roleId, dto.solution
            )
        );
        event.reply(dpp::message("OOPS! Something went wrong! Please contact @erarnitox with this error code: 298364").set_flags(dpp::m_ephemeral));
        return; 
    }
    
    const auto entered_variant{ event.components[0].components[0].value };
    const auto entered_ptr { std::get_if<std::string>(&entered_variant) };
    if(!entered_ptr){
        bot.log(
            dpp::ll_warning, "Corrupted Data occured in ChallengeRoleCommand::handleFormSubmits"
        );
        event.reply(dpp::message("OOPS! Something went wrong! Please contact @erarnitox with this error code: 298365").set_flags(dpp::m_ephemeral));
        return;
    }


    const auto& entered{ *entered_ptr };

    if(entered == dto.solution) {
        bot.guild_member_add_role(event.command.guild_id, member.user_id, dto.roleId);

        event.reply(dpp::message(fmt::format("Well done {}, you solved this challenge!", member.get_mention())).set_flags(dpp::m_ephemeral));
    } else {
        event.reply(dpp::message(fmt::format("Sorry {}, this is not the right answer!", member.get_mention())).set_flags(dpp::m_ephemeral));
    }
}

// UNIMPLEMENTED EVENTS:

// user management
auto ChallengeRoleCommand::welcomeMember(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void {
    (void)event;
    (void)bot;
};

auto ChallengeRoleCommand::leaveMember(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void {
    (void)event;
    (void)bot;
}

// handle added reactions
auto ChallengeRoleCommand::handleReactionAdded(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void {
    (void) event;
    (void) bot;
}

// handle removed reactions
auto ChallengeRoleCommand::handleReactionRemoved(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void {
    (void) event;
    (void) bot;
}