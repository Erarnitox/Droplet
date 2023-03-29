#include <dpp/colors.h>
#pragma #once

#include <vector>
#include <dpp/dpp.h>

namespace core {
    static std::map<dpp::snowflake, dpp::snowflake> welcome_channels;
    static std::map<dpp::snowflake, dpp::snowflake> goodbye_channels;

    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        dpp::slashcommand help_command("help", "Usage information", bot.me.id);
        dpp::slashcommand channel_command("set_channel", "Configure channels for bot events", bot.me.id);
        channel_command.add_option(
	        dpp::command_option(dpp::co_string, "type", "For what should this channel be used?", true)
	            .add_choice(dpp::command_option_choice("Welcome Message Channel", std::string("channel_welcome")))
	            .add_choice(dpp::command_option_choice("Goodbye Message Channel", std::string("channel_goodbye")))
        );

        command_list.push_back(help_command);
        command_list.push_back(channel_command);
    }

    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
        if (event.command.get_command_name() == "help") {
            /* create the embed */
	        dpp::embed embed{ dpp::embed()
	                .set_color(dpp::colors::discord_black)
	                .set_title("CyberDrop - Help")
	                .set_url("https://cyberdrop.dropsoft.org/")
	                .set_description("Usage Information for the CyberDrop Discord bot")
	                .set_thumbnail("https://www.dropsoft.org/img/logo_huc21a5771e65b8d5ba9ff88b74b45cd86_105986_288x288_fill_box_center_3.png")
	                .add_field(
	                        "/resources",
	                        "A resouce manager to keep track of shared resources. Added resources can be found on https://dropsoft.org/resources"
	                )
                    .add_field(
                        "/user_info",
                        "Query information about a user"
                    )
                    .add_field(
                        "/set_channel (ADMIN ONLY)",
                        "Configure channels for bot usage"
                    )
            };
	 
	        /* reply with the created embed */
            event.reply("Here is the usage manual!");
	        bot.message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
            return;
        }
        else if (event.command.get_command_name() == "set_channel") {
            if(event.command.get_issuing_user().id != event.command.get_guild().owner_id){
                event.reply(std::string("You are not the owner!"));
            }

            auto type{ std::get<std::string>(event.get_parameter("type")) };
            
            if(type == "channel_welcome") {
                welcome_channels[event.command.get_guild().id] = event.command.get_channel().id;
                event.reply(std::string("channel set as welcome channel!"));
            }
            else if(type == "channel_goodbye") {
                goodbye_channels[event.command.get_guild().id] = event.command.get_channel().id;
                event.reply(std::string("channel set as goodbye channel!"));
            }

            else event.reply("Ooops! Something went wrong!");
        }
    }
}