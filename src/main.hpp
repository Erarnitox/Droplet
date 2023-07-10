#pragma once

#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <fstream>

#include "core.hpp"
#include "database.hpp"
#include "resource_man.hpp"
#include "user_man.hpp"

[[nodiscard]] static inline
auto read_bot_token(const std::string& file) -> std::string;

[[nodiscard]] static inline
auto read_database_credentials(const std::string& file) -> std::string;

// slash commands
static inline
auto register_global_slash_commands(std::vector<dpp::slashcommand>& command_list, const dpp::cluster& bot) -> void;

static inline
auto handle_global_slash_commands(const dpp::slashcommand_t& event, dpp::cluster& bot, const std::vector<dpp::slashcommand>& command_list) -> void;

// user management
static inline
auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void;

static inline
auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void;

// button clicks
static inline
auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void;

// form submits
static inline
auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot) -> void;

// handle added reactions
static inline
auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void;

// handle removed reactions
static inline
auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void;