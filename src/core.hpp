#include <dpp/colors.h>
#pragma #once

#include <vector>
#include <dpp/dpp.h>

namespace core {
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        dpp::slashcommand help_command("help", "Usage information", bot.me.id);
        command_list.push_back(help_command);
    }

    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
        if (event.command.get_command_name() == "help") {
            /* create the embed */
	        dpp::embed embed{ dpp::embed()
	                .set_color(dpp::colors::deep_sea)
	                .set_title("CyberDrop - Help")
	                .set_url("https://cyberdrop.dropsoft.org/")
	                .set_description("Usage Information for the CyberDrop Discord bot")
	                .set_thumbnail("https://www.dropsoft.org/img/logo_huc21a5771e65b8d5ba9ff88b74b45cd86_105986_288x288_fill_box_center_3.png")
	                .add_field(
	                        "/resources",
	                        "A resouce manager to keep track of shared resources. Added resources can be found on https://dropsoft.org/resources"
	                )
	                .set_footer(dpp::embed_footer().set_text("Some footer text here").set_icon("https://dpp.dev/DPP-Logo.png"))
            };
	 
	        /* reply with the created embed */
	        bot.message_create(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
        }
    }
}