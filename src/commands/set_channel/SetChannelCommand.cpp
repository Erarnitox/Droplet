#include "SetChannelCommand.hpp"

#include <Core.hpp>
#include <Commands.hpp>

#include <fmt/core.h>
#include <variant>

auto SetChannelCommand::registerGlobalSlashCommand(std::vector<dpp::slashcommand> &command_list, const dpp::cluster &bot) noexcept -> void {
    dpp::slashcommand channel_command("set_channel", "Configure channels for bot events (Admin only!)", bot.me.id);
    channel_command.add_option(
	    dpp::command_option(dpp::co_string, "type", "For what should this channel be used?", true)
	        .add_choice(dpp::command_option_choice("Welcome Message Channel", std::string("channel_welcome")))
	        .add_choice(dpp::command_option_choice("Goodbye Message Channel", std::string("channel_goodbye")))
	        .add_choice(dpp::command_option_choice("Server Logging Channel", std::string("channel_log")))
    );

    command_list.push_back(channel_command);
}

auto SetChannelCommand::handleGlobalSlashCommand(const dpp::slashcommand_t& event, dpp::cluster& bot, const std::vector<dpp::slashcommand>& command_list) noexcept -> void {
    // unneeded arguments:
    (void) bot;
    (void) command_list;

    if (event.command.get_command_name() != "set_channel") return;
        
    if(!Core::isAdmin(event.command.member)){
        Core::timedReply(bot, event, std::string("Only admins are allowed to use this command!"), 2000);
        return;
    }

    const auto type{ Core::getParameter(bot, event, "type") };
    if(type.empty()) return;

    if(type == "channel_welcome") {
        Database::insert_welcome_channel_id(event.command.get_guild().id, event.command.get_channel().id);
        Core::timedReply(bot, event, std::string("channel set as welcome channel!"), 2000);
    } else if(type == "channel_goodbye") {
        Database::insert_goodbye_channel_id(event.command.get_guild().id, event.command.get_channel().id);
        Core::timedReply(bot, event, std::string("channel set as goodbye channel!"), 2000);
    } else if(type == "channel_log") {
        Database::insert_log_channel_id(event.command.get_guild().id, event.command.get_channel().id);
        Core::timedReply(bot, event, "Channnel set as logging channel!", 2000);
    }
}

auto SetChannelCommand::handleButtonClicks(const dpp::button_click_t& event, dpp::cluster& bot) noexcept -> void {
    (void) event;
    (void) bot;
}

auto SetChannelCommand::handleFormSubmits(const dpp::form_submit_t& event, dpp::cluster& bot) noexcept -> void {
    (void) event;
    (void) bot;
}

// user management
auto SetChannelCommand::welcomeMember(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void {
    (void)event;
    (void)bot;
};

auto SetChannelCommand::leaveMember(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void {
    (void)event;
    (void)bot;
}

// handle added reactions
auto SetChannelCommand::handleReactionAdded(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void {
    (void) event;
    (void) bot;
}

// handle removed reactions
auto SetChannelCommand::handleReactionRemoved(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void {
    (void) event;
    (void) bot;
}