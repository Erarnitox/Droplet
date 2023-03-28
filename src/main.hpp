#pragma once

#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <fstream>

#include "core.hpp"
#include "ctf_man.hpp"
#include "ctf_solver.hpp"
#include "custom_commands.hpp"
#include "customization.hpp"
#include "podcasts.hpp"
#include "resource_man.hpp"
#include "server_stats.hpp"
#include "user_man.hpp"

auto read_bot_token(const std::string& file) -> std::string;

// resource management
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void;
auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot) -> void;

// user management
auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void;
auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void;