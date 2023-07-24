#pragma once

#include <Core.hpp>
#include <Database.hpp>

#include <dpp/colors.h>
#include <dpp/message.h>
#include <dpp/restresults.h>
#include <fmt/core.h>
#include <stdexcept>
#include <string>
#include <stack>

class ChallengeRoleCommand {

public:
    static inline
    auto registerGlobalSlashCommand(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) noexcept -> void;

    static inline
    auto handleGlobalSlashCommand(const dpp::slashcommand_t& event, dpp::cluster& bot, const std::vector<dpp::slashcommand>& command_list) noexcept -> void;
    
    static inline
    auto handleButtonClicks(const dpp::button_click_t& event, dpp::cluster& bot) noexcept -> void;

    static inline
    auto handleFormSubmits(const dpp::form_submit_t& event, dpp::cluster& bot) noexcept -> void;


    // UNIMPLEMENTED EVENTS:

    // user management
    static inline
    auto welcomeMember(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void;

    static inline
    auto leaveMember(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void;
    
    static inline
    auto handleReactionAdded(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void;

    static inline
    auto handleReactionRemoved(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void;
};