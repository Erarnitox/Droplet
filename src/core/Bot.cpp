#include "Bot.hpp"

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

void Bot::set_token(const std::string& token) {
	ctx.token = token;
}

// slash commands
static inline void register_global_slash_commands(const dpp::cluster& ctx) {
	(void)ctx;
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
		// const std::string& command_name = event.msg.content;

		for (const auto& command : message_commands) {
			command->on_message_create(event);
		}
	});
}

static inline void handle_message_delete(dpp::cluster& ctx, const message_commands_t& message_commands) {
	(void)ctx;
	(void)message_commands;
}

static inline void handle_message_delete_bulk(dpp::cluster& ctx, const message_commands_t& message_commands) {
	(void)ctx;
	(void)message_commands;
}

// user management
static inline void handle_guild_member_add(dpp::cluster& ctx, const member_commands_t& member_commands) {
	(void)ctx;
	(void)member_commands;
}

static inline void handle_guild_member_remove(dpp::cluster& ctx, const member_commands_t& member_commands) {
	(void)ctx;
	(void)member_commands;
}

// button clicks
static inline void handle_button_click(dpp::cluster& ctx, const button_commands_t& button_commands) {
	(void)ctx;
	(void)button_commands;
}

// form submits
static inline void handle_form_submit(dpp::cluster& ctx, const form_commands_t& form_commands) {
	(void)ctx;
	(void)form_commands;
}

// handle added reactions
static inline void handle_reaction_add(dpp::cluster& ctx, const reaction_commands_t& reaction_commands) {
	(void)ctx;
	(void)reaction_commands;
}

// handle removed reactions
static inline void handle_reaction_remove(dpp::cluster& ctx, const reaction_commands_t& reaction_commands) {
	(void)ctx;
	(void)reaction_commands;
}

void Bot::run() {
	// slash commands
	register_global_slash_commands(Bot::ctx);
	handle_global_slash_commands(Bot::ctx, Bot::slash_commands);

	// message commands
	handle_message_create(Bot::ctx, Bot::message_commands);
	handle_message_delete(Bot::ctx, Bot::message_commands);
	handle_message_delete_bulk(Bot::ctx, Bot::message_commands);

	// guild members
	handle_guild_member_add(Bot::ctx, Bot::member_commands);
	handle_guild_member_remove(Bot::ctx, Bot::member_commands);

	// button clicks
	handle_button_click(Bot::ctx, Bot::button_commands);

	// form submits
	handle_form_submit(Bot::ctx, Bot::form_commands);

	// reaction commands
	handle_reaction_add(Bot::ctx, Bot::reaction_commands);
	handle_reaction_remove(Bot::ctx, Bot::reaction_commands);

	ctx.start(dpp::st_wait);
	return;
}