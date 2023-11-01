#include "Bot.hpp"

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>

void Bot::set_token(const std::string& token) {
	ctx.token = token;
}

void Bot::add_slash_command(const std::string& name, const std::shared_ptr<IGlobalSlashCommand>& slash_command) {
	Bot::slash_commands[name] = slash_command;
}

void Bot::add_button_command(const std::shared_ptr<IButtonCommand>& button_command) {
	Bot::button_commands.push_back(button_command);
}

void Bot::add_form_command(const std::shared_ptr<IFormCommand>& form_command) {
	Bot::form_commands.push_back(form_command);
}

void Bot::add_member_command(const std::shared_ptr<IMemberCommand>& member_command) {
	Bot::member_commands.push_back(member_command);
}

void Bot::add_message_command(const std::shared_ptr<IMessageCommand>& message_command) {
	Bot::message_commands.push_back(message_command);
}

void Bot::add_reaction_command(const std::shared_ptr<IReactionCommand>& reaction_command) {
	Bot::reaction_commands.push_back(reaction_command);
}

void Bot::add_ready_command(const std::shared_ptr<IReady>& ready_command) {
	Bot::ready_commands.push_back(ready_command);
}

// slash commands
static inline void register_global_slash_commands(dpp::cluster& ctx, const slash_commands_t& slash_commands) {
	ctx.on_ready([&ctx, &slash_commands](const dpp::ready_t& event) -> void {
		(void)event;

		ctx.log(dpp::ll_trace, "Registering Slash commands...");

		if (dpp::run_once<struct register_bot_commands>()) {
			for (const auto& slash_command : slash_commands) {
				dpp::slashcommand tmp_command(
					slash_command.first, slash_command.second->command_description, ctx.me.id);
				ctx.global_command_create(tmp_command);
			}
		}
	});
}

static inline void handle_on_log(dpp::cluster& ctx) {
	//-----------------------------------------------------------------------------
	// Functionality for Logging
	//-----------------------------------------------------------------------------
	auto err_log_out{
		fmt::output_file("error.log", fmt::file::WRONLY | fmt::file::CREATE | fmt::file::APPEND | fmt::file::TRUNC)};
	auto log_out{
		fmt::output_file("droplet.log", fmt::file::WRONLY | fmt::file::CREATE | fmt::file::APPEND | fmt::file::TRUNC)};

	ctx.on_log([&log_out, &err_log_out](const dpp::log_t& event) {
		if (event.severity == dpp::ll_trace) {
			fmt::print(
				fg(fmt::color::green), "[TRACE]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_debug) {
			fmt::print(
				fg(fmt::color::blue), "[DEBUG]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_info) {
			fmt::print(fg(fmt::color::antique_white),
					   "[INFO]\t\t({})\t\t\"{}\"\n",
					   dpp::utility::current_date_time(),
					   event.message);
			log_out.print("[INFO]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_warning) {
			fmt::print(fg(fmt::color::yellow),
					   "[WARNING]\t({})\t\t\"{}\"\n",
					   dpp::utility::current_date_time(),
					   event.message);
			log_out.print("[WARNING]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_error) {
			fmt::print(fg(fmt::color::orange_red),
					   "[ERROR]\t\t({})\t\t\"{}\"\n",
					   dpp::utility::current_date_time(),
					   event.message);
			log_out.print("[ERROR]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
			err_log_out.print("[ERROR]\t\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		} else if (event.severity == dpp::ll_critical) {
			fmt::print(
				fg(fmt::color::red), "[CRITICAL]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
			log_out.print("[CRITICAL]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
			err_log_out.print("[CRITICAL]\t({})\t\t\"{}\"\n", dpp::utility::current_date_time(), event.message);
		}

		if (event.severity > dpp::ll_debug) {
			log_out.flush();
			err_log_out.flush();
		}
	});
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
		for (const auto& command : message_commands) {
			command->on_message_create(event);
		}
	});
}

static inline void handle_message_delete(dpp::cluster& ctx, const message_commands_t& message_commands) {
	ctx.on_message_delete([&message_commands](const dpp::message_delete_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete(event);
		}
	});
}

static inline void handle_message_delete_bulk(dpp::cluster& ctx, const message_commands_t& message_commands) {
	ctx.on_message_delete_bulk([&message_commands](const dpp::message_delete_bulk_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete_bulk(event);
		}
	});
}

// user management
static inline void handle_guild_member_add(dpp::cluster& ctx, const member_commands_t& member_commands) {
	ctx.on_guild_member_add([&member_commands](const dpp::guild_member_add_t& event) {
		for (const auto& command : member_commands) {
			command->on_guild_member_add(event);
		}
	});
}

static inline void handle_guild_member_remove(dpp::cluster& ctx, const member_commands_t& member_commands) {
	ctx.on_guild_member_remove([&member_commands](const dpp::guild_member_remove_t& event) {
		for (const auto& command : member_commands) {
			command->on_guild_member_remove(event);
		}
	});
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

// handle ready event
static inline void handle_ready(dpp::cluster& ctx, const ready_commands_t& ready_commands) {
	(void)ctx;
	(void)ready_commands;
}

void Bot::run() {
	// custom logger
	handle_on_log(Bot::ctx);

	// slash commands
	register_global_slash_commands(Bot::ctx, Bot::slash_commands);
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

	// when bot is ready
	handle_ready(Bot::ctx, Bot::ready_commands);

	ctx.start(dpp::st_wait);
	return;
}