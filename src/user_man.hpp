#pragma once

#include <dpp/dpp.h>

namespace user_man{
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        dpp::slashcommand help_command("user_info", "Query Information about a user", bot.me.id);
        help_command.add_option(
	        dpp::command_option(dpp::co_string, "user", "User you would like information about", true)
        );
        
        command_list.push_back(help_command);
    }

    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
        if (event.command.get_command_name() == "user_info") {
            event.reply("Not implemented yet!");
        }
    }

    auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void {
          /* create the embed */
	        dpp::embed embed{ dpp::embed()
	                .set_color(dpp::colors::deep_sea)
	                .set_title(event.adding_guild->name)
	                .set_description(std::string("Welcome ") 
                        + event.added.get_mention()
                        + " to "
                        + event.adding_guild->name 
                    )
                    .set_image(event.added.get_avatar_url())
            };
	 
	        /* reply with the created embed */
	        bot.message_create(dpp::message(core::welcome_channels.at(event.adding_guild->id), embed));
    }

    auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void {

    }
}