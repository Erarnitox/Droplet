#include "Core.hpp"

#include <ratio>
#include <variant>
#include <vector>
#include <thread>
#include <chrono>
#include <regex>

inline
auto Core::isAdmin(const dpp::guild_member& member) noexcept -> bool {
    for (const auto& role_id : member.roles) {
        const dpp::role& role{ *dpp::find_role(role_id) };
        if(role.has_administrator()) return true;
    }
    return false;
}

inline
auto Core::getRoleId(const std::string& mention) noexcept -> std::string {
    std::regex re("<@&([0-9]+)>");
    std::smatch match;

    if (std::regex_search(mention, match, re)) {
        return match.str(1);
    } else {
        return "";
    }
}

inline
auto Core::getChannelId(const std::string& mention) noexcept -> std::string {
    std::regex re("<#([0-9]+)>");
    std::smatch match;

    if (std::regex_search(mention, match, re)) {
        return match.str(1);
    } else {
        return "";
    }
}

inline
auto Core::timedReply(const dpp::slashcommand_t& event, 
const std::string& message, size_t time_mills) noexcept -> void {
    event.reply(message);

    std::thread([event, time_mills]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_mills));
        event.delete_original_response();
    }).detach();
}

inline
auto Core::timedReply(const dpp::form_submit_t& event,
const std::string& message, size_t time_mills) noexcept -> void {
    event.reply(message);

    std::thread([event, time_mills]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_mills));
        event.delete_original_response();
    }).detach();
}
    
inline
auto Core::getParameter(const dpp::slashcommand_t& event, const std::string& name) noexcept -> std::string {
    const auto variant{ event.get_parameter(name) }; 

    const auto value_ptr{ std::get_if<std::string>(&variant) };
    if(!value_ptr){
        Core::timedReply(event, "Message Link not valid!", 2000);
        return std::string("");
    } 
    return *value_ptr;
}

inline
auto Core::registerGlobalSlashCommands(std::vector<dpp::slashcommand>& command_list, 
const dpp::cluster& bot) noexcept -> void {
    dpp::slashcommand help_command("help", "Usage information", bot.me.id);
        
    dpp::slashcommand channel_command("set_channel", "Configure channels for bot events (Admin only!)", bot.me.id);
    channel_command.add_option(
	    dpp::command_option(dpp::co_string, "type", "For what should this channel be used?", true)
	        .add_choice(dpp::command_option_choice("Welcome Message Channel", std::string("channel_welcome")))
	        .add_choice(dpp::command_option_choice("Goodbye Message Channel", std::string("channel_goodbye")))
	        .add_choice(dpp::command_option_choice("Server Logging Channel", std::string("channel_log")))
    );

    command_list.push_back(help_command);
    command_list.push_back(channel_command);
}

inline
auto Core::handleGlobalSlashCommands(const dpp::slashcommand_t& event, dpp::cluster& bot, 
const std::vector<dpp::slashcommand>& command_list) noexcept -> void {
    if (event.command.get_command_name() == "help") {
        /* create the embed */
	    dpp::embed embed{ dpp::embed()
	        .set_color(dpp::colors::discord_black)
	        .set_title("Droplet - Help") //TODO: generate dynamically
	        .set_url("https://droplet.dropsoft.org/")
	        .set_description("Usage Information for the Droplet Discord bot")
	        .set_thumbnail("https://www.dropsoft.org/img/logo_huc21a5771e65b8d5ba9ff88b74b45cd86_105986_288x288_fill_box_center_3.png")
        };

        for(auto& command : command_list) {
            if(!Core::isAdmin(event.command.member) && command.description.ends_with("(Admin only!)")) continue;
                
            auto& options{ command.options };
            std::string options_string;

            for(auto& option : options) {
                if(option.required) options_string.append(fmt::format(" <{}>", option.name));
                else options_string.append(fmt::format(" [{}]", option.name));
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
        if(!Core::isAdmin(event.command.member)){
            Core::timedReply(event, std::string("Only admins are allowed to use this command!"), 2000);
            return;
        }

        const auto type{ Core::getParameter(event, "type") };
        if(type.empty()) return;

        if(type == "channel_welcome") {
            Database::insert_welcome_channel_id(event.command.get_guild().id, event.command.get_channel().id);
            Core::timedReply(event, std::string("channel set as welcome channel!"), 2000);
        } else if(type == "channel_goodbye") {
            Database::insert_goodbye_channel_id(event.command.get_guild().id, event.command.get_channel().id);
            Core::timedReply(event, std::string("channel set as goodbye channel!"), 2000);
        } else if(type == "channel_log") {
            Database::insert_log_channel_id(event.command.get_guild().id, event.command.get_channel().id);
            Core::timedReply(event, "Channnel set as logging channel!", 2000);
        }
    }
}