#pragma once

#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <fstream>

#include "core.hpp"
#include "ctf_man.hpp"
#include "ctf_solver.hpp"
#include "custom_commands.hpp"
#include "customization.hpp"
#include "database.hpp"
#include "podcasts.hpp"
#include "resource_man.hpp"
#include "server_stats.hpp"
#include "user_man.hpp"

auto read_bot_token(const std::string& file) -> std::string;
auto read_database_credentials(const std::string& file) -> std::string;

// slash commands
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void;
auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot, const std::vector<dpp::slashcommand>& command_list, Database& db) -> void;

// user management
auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void;
auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void;

// button clicks
auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void;

// form submits
auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot, Database& db) -> void;

// handle added reactions
auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void;

// handle removed reactions
auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void;