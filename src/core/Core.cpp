#include "Core.hpp"

#include <dpp/permissions.h>
#include <dpp/timer.h>
#include <ratio>
#include <variant>
#include <vector>
#include <thread>
#include <chrono>
#include <regex>

auto Core::isAdmin(const dpp::guild_member& member) noexcept -> bool {
    for (const auto& role_id : member.get_roles()) {
        const dpp::role& role{ *dpp::find_role(role_id) };
        if(role.has_administrator()) return true;
    }
    return false;
}

auto Core::getRoleId(const std::string& mention) noexcept -> std::string {
    std::regex re("<@&([0-9]+)>");
    std::smatch match;

    if (std::regex_search(mention, match, re)) {
        return match.str(1);
    } else {
        return "";
    }
}

auto Core::getChannelId(const std::string& mention) noexcept -> std::string {
    std::regex re("<#([0-9]+)>");
    std::smatch match;

    if (std::regex_search(mention, match, re)) {
        return match.str(1);
    } else {
        return "";
    }
}

template<typename CMD_TYPE>
auto timedReplyTemplate(dpp::cluster& bot, const CMD_TYPE event, 
const std::string& message, size_t time_mills) noexcept -> void {
    event.reply(message);

    dpp::timer_callback_t on_tick = [&bot, event](dpp::timer deleteTimer){
        event.delete_original_response();
        bot.stop_timer(deleteTimer);
    };

    bot.start_timer(on_tick, time_mills/1000 );
}


auto Core::timedReply(dpp::cluster& bot, const dpp::slashcommand_t event, 
const std::string& message, size_t time_mills) noexcept -> void {
    timedReplyTemplate<dpp::slashcommand_t>(bot, event, message, time_mills);
}

auto Core::timedReply(dpp::cluster& bot, const dpp::form_submit_t event, 
const std::string& message, size_t time_mills) noexcept -> void {
    timedReplyTemplate<dpp::form_submit_t>(bot, event, message, time_mills);
}

auto Core::getParameter(dpp::cluster& bot, const dpp::slashcommand_t event, const std::string& name) noexcept -> std::string {
    const auto variant{ event.get_parameter(name) }; 

    const auto value_ptr{ std::get_if<std::string>(&variant) };
    if(!value_ptr){
        Core::timedReply(bot, event, "Message Link not valid!", 2000);
        return std::string("");
    } 
    return *value_ptr;
}