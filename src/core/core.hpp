#pragma once

#include <dpp/dispatcher.h>
#include <dpp/misc-enum.h>
#include <ratio>
#include <variant>
#include <vector>
#include <thread>
#include <chrono>
#include <regex>

#include <fmt/core.h>
#include <dpp/dpp.h>

#include "database/database.hpp"

namespace core {
    
    static
    auto is_admin(const dpp::guild_member& member) noexcept -> bool {
        for (const auto& role_id : member.roles) {
            const dpp::role& role{ *dpp::find_role(role_id) };
            if(role.has_administrator())
                return true;
        }
        return false;
    }

    static
    auto get_role_id(const std::string& mention) noexcept -> std::string {
        std::regex re("<@&([0-9]+)>");
        std::smatch match;

        if (std::regex_search(mention, match, re)) {
            return match.str(1);
        } else {
            return "";
        }
    }

    static
    auto get_channel_id(const std::string& mention) noexcept -> std::string {
        std::regex re("<#([0-9]+)>");
        std::smatch match;

        if (std::regex_search(mention, match, re)) {
            return match.str(1);
        } else {
            return "";
        }
    }

    static 
    auto timed_reply(const dpp::slashcommand_t& event, const std::string& message, size_t time_mills) noexcept -> void {
        // Get the interaction response object
        // event.reply(LanguageServer::get(event.command.guild_id, message_token));
        event.reply(message);

        std::thread([event, time_mills]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(time_mills));
            event.delete_original_response();
        }).detach();
    }

    static 
    auto timed_reply(const dpp::form_submit_t& event, const std::string& message, size_t time_mills) noexcept -> void {
        event.reply(message);

        std::thread([event, time_mills]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(time_mills));
            event.delete_original_response();
        }).detach();
    }
    
    [[nodiscard]] static inline
    auto get_parameter(const dpp::slashcommand_t& event, const std::string& name) noexcept -> std::string {
            const auto variant{ event.get_parameter(name) }; 

            const auto value_ptr{ std::get_if<std::string>(&variant) };
            if(!value_ptr){
                core::timed_reply(event, "Message Link not valid!", 2000);
                return std::string("");
            } 
            return *value_ptr;
    }

    static inline
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) noexcept -> void {
        dpp::slashcommand help_command("help", "Usage information", bot.me.id);
        //help_command.add_localization("de", "hilfe", "Hilfestellung zur Benutzung des Bots")

        dpp::slashcommand channel_command("set_channel", "Configure channels for bot events (Admin only!)", bot.me.id);
        channel_command.add_option(
	        dpp::command_option(dpp::co_string, "type", "For what should this channel be used?", true)
	            .add_choice(dpp::command_option_choice("Welcome Message Channel", std::string("channel_welcome")))
	            .add_choice(dpp::command_option_choice("Goodbye Message Channel", std::string("channel_goodbye")))
	            .add_choice(dpp::command_option_choice("Server Logging Channel", std::string("channel_log")))
        );

        dpp::slashcommand language_command("set_language", "Configue the language being used by the bot (Admin only!)", bot.me.id);
        language_command.add_option(
            dpp::command_option(dpp::co_string, "language", "What language should be used?", true)
	            .add_choice(dpp::command_option_choice("German", std::string("de")))
	            .add_choice(dpp::command_option_choice("English", std::string("en-US")))
        );

        command_list.push_back(help_command);
        command_list.push_back(channel_command);
        command_list.push_back(language_command);
    }

    static inline
    auto handle_global_slash_commands(
        const dpp::slashcommand_t& event, 
        dpp::cluster& bot, 
        const std::vector<dpp::slashcommand>& command_list
    ) noexcept -> void {
        if (event.command.get_command_name() == "help") {
            /* create the embed */
	        dpp::embed embed{ dpp::embed()
	                .set_color(dpp::colors::discord_black)
	                .set_title("CyberDrop - Help")
	                .set_url("https://cyberdrop.dropsoft.org/")
	                .set_description("Usage Information for the CyberDrop Discord bot")
	                .set_thumbnail("https://www.dropsoft.org/img/logo_huc21a5771e65b8d5ba9ff88b74b45cd86_105986_288x288_fill_box_center_3.png")
            };

            for(auto& command : command_list) {
                if(!core::is_admin(event.command.member) && command.description.ends_with("(Admin only!)")) continue;
                
                auto& options{ command.options };
                std::string options_string;

                for(auto& option : options) {
                    if(option.required)
                        options_string.append(fmt::format(" <{}>", option.name));
                    else
                        options_string.append(fmt::format(" [{}]", option.name));
                }
                embed.add_field(
                    std::string("/")
                        .append(command.name)
                        .append(options_string),
                    command.description
                );
            }
	 
	        /* reply with the created embed */
            event.reply("Here is the usage manual!");
	        bot.message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
            return;
        } else if (event.command.get_command_name() == "set_channel") {
            if(!is_admin(event.command.member)){
                timed_reply(event, std::string("Only admins are allowed to use this command!"), 2000);
                return;
            }

            const auto type{ core::get_parameter(event, "type") };
            if(type.empty()) return;

            if(type == "channel_welcome") {
                Database::insert_welcome_channel_id(event.command.get_guild().id, event.command.get_channel().id);
                timed_reply(event, std::string("channel set as welcome channel!"), 2000);
            } else if(type == "channel_goodbye") {
                Database::insert_goodbye_channel_id(event.command.get_guild().id, event.command.get_channel().id);
                timed_reply(event, std::string("channel set as goodbye channel!"), 2000);
            } else if(type == "channel_log") {
                Database::insert_log_channel_id(event.command.get_guild().id, event.command.get_channel().id);
                timed_reply(event, "Channnel set as logging channel!", 2000);
            }
        } else if (event.command.get_command_name() == "set_language") {
            timed_reply(event, std::string("Currently only English has a complete token set.\nMore languages will be added soon!"), 5000);
        }
    }
}