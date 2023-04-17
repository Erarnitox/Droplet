#pragma once

#include <dpp/dpp.h>
#include "core.hpp"

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

	    dpp::slashcommand res_command("resources", "Resource collection managemant", bot.me.id);
	    
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

        command_list.push_back(res_command);
        
        return;
    }

    auto handle_global_slash_commands(const dpp::slashcommand_t& event) -> void {
        if (event.command.get_command_name() == "resources") {
	        const auto& action{ std::get<std::string>(event.get_parameter("action")) };
            const auto& category{ std::get<std::string>(event.get_parameter("category")) };
            const auto& title{ std::get<std::string>(event.get_parameter("title")) };
            const auto& query{ std::get<std::string>(event.get_parameter("query")) };
            const auto& index{ std::get<std::string>(event.get_parameter("index")) };
            const auto& link{ std::get<std::string>(event.get_parameter("link")) };
            const auto& description{ std::get<std::string>(event.get_parameter("description")) };
	        
            if(action.size() < 1){
                core::timed_reply(event, "Please specify an action!", 100);
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