#pragma #once

#include <vector>
#include <dpp/dpp.h>

namespace core {
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        dpp::slashcommand help_command("help", "Usage information", bot.me.id);
        command_list.push_back(help_command);
    }

    auto handle_global_slash_commands(const dpp::slashcommand_t& event, const dpp::cluster& bot) -> void {
        
    }
}