#pragma once

#include <dpp/misc-enum.h>

#include <fmt/core.h>
#include <dpp/dpp.h>

#include "database/Database.hpp"

class Core {
public:
    static
    auto isAdmin(const dpp::guild_member& member) noexcept -> bool;

    static
    auto getRoleId(const std::string& mention) noexcept -> std::string;

    static
    auto getChannelId(const std::string& mention) noexcept -> std::string;

    static
    auto timedReply(const dpp::slashcommand_t& event, const std::string& message, size_t time_mills) noexcept -> void;

    static
    auto timedReply(const dpp::form_submit_t& event, const std::string& message, size_t time_mills) noexcept -> void;
    
    [[nodiscard]] static
    auto getParameter(const dpp::slashcommand_t& event, const std::string& name) noexcept -> std::string;

    //TODO: Remove
    static
    auto registerGlobalSlashCommands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) noexcept -> void;

    //TODO: Remove
    static
    auto handleGlobalSlashCommands(const dpp::slashcommand_t& event, dpp::cluster& bot, 
        const std::vector<dpp::slashcommand>& command_list) noexcept -> void;

};