include<core.hpp>
#include "ChallengeRoleCommand.hpp"

static inline 
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

static inline
auto ChallengeRoleCommand::handleGlobalSlashCommand(const dpp::slashcommand_t& event, dpp::cluster& bot) noexcept -> void {
    if (event.command.get_command_name() != "challenge_role") return;
    
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
            ChallengRoleRepository repo;
            ChallengeRoleDto data { sane_role_id, guild_id, messsage_id, solution };
            if(repo.create(data)){
                // send a confirmation to the admin
                core::timed_reply(
                    event, 
                    fmt::format(
                        "Challenge Created!\nQuestion: {}\nReward: {}",
                        question, role
                    ), 
                    10000 //10sek
                );
            } else {
                // send a confirmation to the admin
                core::timed_reply(
                    event, 
                    "Something went wrong creating the Challenge :c", 
                    10000 //10sek
                );
            }
    );
}

    static inline
    auto handleButtonClicks(const dpp::button_click_t& event, dpp::cluster& bot) noexcept -> void {
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
    auto handleFormSubmits(const dpp::form_submit_t& event, dpp::cluster& bot) noexcept -> void {
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