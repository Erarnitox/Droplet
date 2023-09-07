#include "Core.hpp"

#include <ratio>
#include <variant>
#include <vector>
#include <thread>
#include <chrono>
#include <regex>

auto Core::isAdmin(const dpp::guild_member& member) noexcept -> bool {
    for (const auto& role_id : member.roles) {
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

auto Core::timedReply(const dpp::slashcommand_t& event, 
const std::string& message, size_t time_mills) noexcept -> void {
    event.reply(message);

    std::thread([event, time_mills]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_mills));
        event.delete_original_response();
    }).detach();
}

auto Core::timedReply(const dpp::form_submit_t& event,
const std::string& message, size_t time_mills) noexcept -> void {
    event.reply(message);

    std::thread([event, time_mills]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_mills));
        event.delete_original_response();
    }).detach();
}
    
auto Core::getParameter(const dpp::slashcommand_t& event, const std::string& name) noexcept -> std::string {
    const auto variant{ event.get_parameter(name) }; 

    const auto value_ptr{ std::get_if<std::string>(&variant) };
    if(!value_ptr){
        Core::timedReply(event, "Message Link not valid!", 2000);
        return std::string("");
    } 
    return *value_ptr;
}