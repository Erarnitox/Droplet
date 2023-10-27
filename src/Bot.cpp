#include "Bot.hpp"

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

#include "core/interfaces/IGlobalSlashCommand.hpp"

void Bot::set_token(const std::string& token) {
	ctx.token = token;
}

// slash commands
static inline void register_global_slash_commands(const dpp::cluster& ctx) {
}

static inline void handle_global_slash_commands(dpp::cluster& ctx, const slash_commands_t& slash_commands) {
	ctx.on_slashcommand([&slash_commands](const dpp::slashcommand_t& event) {
		const std::string& command_name = event.command.get_command_name();
		if (slash_commands.find(command_name) != slash_commands.end()) {
			slash_commands.at(command_name)->on_slashcommand(event);
		}
	});
}

// message commands
static inline void handle_message_create(dpp::cluster& ctx, const message_commands_t& message_commands) {
	ctx.on_message_create([&message_commands](const dpp::message_create_t& event) {
		const std::string& command_name = event.msg.content;
		if (message_commands.find(command_name) != message_commands.end()) {
			message_commands.at(command_name)->on_message_create(event);
		}
	});
}

static inline void handle_message_delete(dpp::cluster& ctx, const message_commands_t& message_commands) {
}

static inline void handle_message_delete_bulk(dpp::cluster& ctx, const message_commands_t& message_commands) {
}

// user management
static inline auto welcome_member(const dpp::guild_member_add_t& event, dpp::cluster& bot) -> void;

static inline auto leave_member(const dpp::guild_member_remove_t& event, dpp::cluster& bot) -> void;

// button clicks
static inline auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void;

// form submits
static inline auto handle_form_submits(const dpp::form_submit_t& event, dpp::cluster& bot) -> void;

// handle added reactions
static inline auto handle_reaction_added(const dpp::message_reaction_add_t& event, dpp::cluster& bot) -> void;

// handle removed reactions
static inline auto handle_reaction_removed(const dpp::message_reaction_remove_t& event, dpp::cluster& bot) -> void;

void Bot::run() {
	// slash commands
	register_global_slash_commands(Bot::ctx);
	handle_global_slash_commands(Bot::ctx, Bot::slash_commands);

	// message commands
	handle_message_create(Bot::ctx, Bot::message_commands);
	handle_message_delete(Bot::ctx, Bot::message_commands);
	handle_message_delete_bulk(Bot::ctx, Bot::message_commands);

	ctx.start(dpp::st_wait);
	return;
}