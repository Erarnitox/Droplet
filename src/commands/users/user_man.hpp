#pragma once

#include <Database.hpp>
#include <dpp/dpp.h>

namespace user_man{
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        dpp::slashcommand help_command("user_info", "Query Information about a user", bot.me.id);
        help_command.add_option(
	        dpp::command_option(dpp::co_string, "user", "User you would like information about", true)
        );

        //command_list.push_back(help_command);
    }

    static inline
    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
        if (event.command.get_command_name() == "user_info") {
            auto mentioned_user{ Core::getParameter(bot, event, "user") };
            if(mentioned_user.empty()) return;
            
            //const dpp::guild_member& member{ event.command.get_guild().members.find(mentioned_user) };
            // find the mentioned user:
            for(const auto& member : event.command.get_guild().members) {
                const auto& user{ member.second.get_user() };

                bot.message_create(dpp::message(event.command.channel_id, "user:" + user->get_mention()));

                if(user->get_mention() == mentioned_user) {
                    /* create the embed */
	                dpp::embed embed{ dpp::embed()
	                    .set_color(dpp::colors::deep_sea)
	                    .set_title("Information about " + user->username)
                        .set_image(user->get_avatar_url())
                        .add_field("Number of guilds", std::to_string(user->refcount))
                    };

                    /* reply with the created embed */
	                event.reply(dpp::message(event.command.channel_id, embed));
                    return;
                }
            }

            event.reply(std::string("There is no information about ") + mentioned_user);
        }
    }

    static inline
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
        const auto channel_id { Database::get_welcome_channel_id(event.adding_guild->id) };
        if (!channel_id) return;

	    bot.message_create(dpp::message(channel_id, embed));
    }

    static inline
    auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void {
        /* create the embed */
	    dpp::embed embed{ dpp::embed()
	        .set_color(dpp::colors::red)
	        .set_title(event.removing_guild->name)
	        .set_description(std::string("Goodbye! ") 
                + event.removed->get_mention()
                + " just left "
                + event.removing_guild->name
            )
            .set_image(event.removed->get_avatar_url())
        };
	 
	    /* reply with the created embed */
        const auto channel_id { Database::get_welcome_channel_id(event.removing_guild->id) };
        if (!channel_id) return;

	    bot.message_create(dpp::message(channel_id, embed));
    }
}