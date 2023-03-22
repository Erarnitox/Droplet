#pragma once

#include <dpp/appcommand.h>
#include <dpp/dpp.h>

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

	    dpp::slashcommand res_command("resman", "Resource collection managemant", bot.me.id);
	    
        // action option
        res_command.add_option(
	        dpp::command_option(dpp::co_string, "action", "What would you like to do?", true)
	            .add_choice(dpp::command_option_choice("add", std::string("action_add")))
	            .add_choice(dpp::command_option_choice("delete", std::string("action_del")))
	            .add_choice(dpp::command_option_choice("list", std::string("action_list")))
                .add_choice(dpp::command_option_choice())
        );

        // category option
        {
            auto co{ dpp::command_option(dpp::co_string, "category", "The category of the resource", true) };

            for(const auto& choice : categories) {
                co.add_choice(dpp::command_option_choice(choice.first, choice.second));
            }
            res_command.add_option(co);
        }

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
        if (event.command.get_command_name() == "resman") {
	            std::string action{ std::get<std::string>(event.get_parameter("animal")) };
                std::string category{ std::get<std::string>(event.get_parameter("category")) };
                std::string index{ std::get<std::string>(event.get_parameter("index")) };
                std::string link{ std::get<std::string>(event.get_parameter("link")) };
                std::string description{ std::get<std::string>(event.get_parameter("description")) };
	           
                if(action == "add") {
	                event.reply(std::string("Resource added: ") + link);
                }
	        }
    }
}