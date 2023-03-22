#pragma once

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
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void;
auto handle_global_slash_commands(const dpp::slashcommand_t& event) -> void;
