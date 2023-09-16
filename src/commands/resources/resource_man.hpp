#pragma once

#include <dpp/dpp.h>
#include <Core.hpp>

namespace resource_man {
    auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void {
        const static std::vector<std::pair<std::string, std::string>> categories {
            { "C++ Programming", "cpp_programming" },
            { "C++ Libraries", "cpp_libraries" },
            { "C++ Tooling", "cpp_tooling" },
            { "Pentesting", "pentesting" },
            { "Other Hacking", "other_hacking" },
            { "Binary Exploitation (Generic)", "general_binary_exploitation" },
            { "Heap based Exploitation", "heap_exploitation" },
            { "Stack based Exploitation", "stack_exploitation" },
            { "Forensics", "forensics" },
            { "Cryptography", "cryptography" },
            { "Blockchain / DeFi", "blockchain" },
            { "Mathematics", "mathematics" },
            { "Hacker Music", "hacker_music" },
            { "Hacker Podcasts", "hacker_podcasts" },
            { "Hacker Entertainment", "hacker_entertainment" },
            { "Reverse Engineering", "reverse_engineering" },
            { "Game Hacking", "game_hacking" },
            { "Hardware (Hacking / Development)", "hardware" },
            { "Malware (Research / Analysis)", "malware_reseach" },
            { "Misc / Other", "misc" }
        };

	    dpp::slashcommand res_command("resources", "Resource collection management", bot.me.id);
	    
        // action option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "action", "What would you like to do?", true)
	            .add_choice(dpp::command_option_choice("add", std::string("action_add")))
	            .add_choice(dpp::command_option_choice("delete", std::string("action_del")))
	            .add_choice(dpp::command_option_choice("list", std::string("action_list")))
                .add_choice(dpp::command_option_choice("list", std::string("action_search")))
        );

        // category option
        {
            auto co{ dpp::command_option(dpp::co_string, "category", "The category of the resource", true) };

            for(const auto& choice : categories) {
                co.add_choice(dpp::command_option_choice(choice.first, choice.second));
            }
            res_command.add_option(co);
        }

        // title option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "title", "The title of the resource", false)
	    );

        // query option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "query", "what to search for", false)
	    );

        // index option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "index", "The index of the resource", false)
	    );

        // link option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "link", "A link to the resource", false)
	    );

        // description option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "description", "A short description of the resource", false)
	    );

        //command_list.push_back(res_command);
        
        return;
    }

    auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void {
        if (event.command.get_command_name() == "resources") {
	        const auto action{ Core::getParameter(bot, event, "action") };
            if(action.empty()) return; 

            const auto category{ Core::getParameter(bot, event, "category") };
            if(category.empty()) return;

            const auto title{ Core::getParameter(bot, event, "title") };
            if(title.empty()) return;
            
            const auto query{ Core::getParameter(bot, event, "query") };
            if(query.empty()) return;

            const auto index{ Core::getParameter(bot, event, "index") };
            if(index.empty()) return;
            
            const auto link{ Core::getParameter(bot, event, "link") };
            if(link.empty()) return;
            
            const auto description{ Core::getParameter(bot, event, "description") };
            if(description.empty()) return;
	        
            if(action.size() < 1){
                Core::timedReply(bot, event, "Please specify an action!", 100);
            } else if(action == "action_add") {
	            // usage: <title> <link> <category> [description]
            } else if(action == "action_del") {
                // usage: <category> <id>
            } else if(action == "action_list") {
                // usage: [category]
            } else if(action == "action_search") {
                // usage: <query>
            }
	    }

        return;
    }
}