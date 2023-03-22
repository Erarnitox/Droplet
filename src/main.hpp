#pragma once

#include <dpp/dpp.h>
#include <fstream>

#include "resource_man/resource_man.hpp"

auto read_bot_token(const std::string& file) -> std::string;
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list) -> void;
auto handle_global_slash_commands(const dpp::slashcommand_t& event) -> void;
