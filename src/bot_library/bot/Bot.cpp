/**
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class handles the execution of the
 *	bot and delegats events to registerd commands.
 *
 *  Documentation: https://droplet.dropsoft.org/doxygen/html/bot
 **/

#include "Bot.hpp"

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/os.h>

// initialize static members
// ctx_t Bot::ctx;
std::string Bot::ctx_token;
button_commands_t Bot::button_commands;
form_commands_t Bot::form_commands;
slash_commands_t Bot::slash_commands;
member_commands_t Bot::member_commands;
message_commands_t Bot::message_commands;
reaction_commands_t Bot::reaction_commands;
ready_commands_t Bot::ready_commands;

/**
 * @brief initializes our bot and sets the bot token
 *
 * @param token the token to be used by the bot
 * @return doesn't return anything
 */
void Bot::init(const std::string& token) {
	Bot::ctx_token = token;
	Bot::button_commands = button_commands_t();
	Bot::form_commands = form_commands_t();
	Bot::slash_commands = slash_commands_t();
	Bot::member_commands = member_commands_t();
	Bot::message_commands = message_commands_t();
	Bot::reaction_commands = reaction_commands_t();
	Bot::ready_commands = ready_commands_t();
}

/**
 * @brief registers new slash commands with the bot
 *
 * @param name the name of the slash command
 * @param slash_command the command that implements the IGlobalSlashCommand interface
 * @return doesn't return anything
 */
void Bot::add_slash_command(const std::string& name, const std::shared_ptr<IGlobalSlashCommand>& slash_command) {
	Bot::slash_commands[name] = slash_command;
}

/**
 * @brief registers new button commands with the bot
 *
 * @param button_command the command that implements the IButtonCommand interface
 * @return doesn't return anything
 */
void Bot::add_button_command(const std::shared_ptr<IButtonCommand>& button_command) {
	Bot::button_commands.push_back(button_command);
}

/**
 * @brief registers new form commands with the bot
 *
 * @param form_command the command that implements the IFormCommand interface
 * @return doesn't return anything
 */
void Bot::add_form_command(const std::shared_ptr<IFormCommand>& form_command) {
	Bot::form_commands.push_back(form_command);
}

/**
 * @brief registers new member commands with the bot
 *
 * @param member_command the command that implements the IMemberCommand interface
 * @return doesn't return anything
 */
void Bot::add_member_command(const std::shared_ptr<IMemberCommand>& member_command) {
	Bot::member_commands.push_back(member_command);
}

/**
 * @brief registers new message commands with the bot
 *
 * @param message_command the command that implements the IMessageCommand interface
 * @return doesn't return anything
 */
void Bot::add_message_command(const std::shared_ptr<IMessageCommand>& message_command) {
	Bot::message_commands.push_back(message_command);
}

/**
 * @brief registers new reaction commands with the bot
 *
 * @param reaction_command the command that implements the IReactionCommand interface
 * @return doesn't return anything
 */
void Bot::add_reaction_command(const std::shared_ptr<IReactionCommand>& reaction_command) {
	Bot::reaction_commands.push_back(reaction_command);
}

/**
 * @brief registers new ready commands with the bot
 *
 * @param ready_command the command that implements the IReady interface
 * @return doesn't return anything
 */
void Bot::add_ready_command(const std::shared_ptr<IReady>& ready_command) {
	Bot::ready_commands.push_back(ready_command);
}

/**
 * @brief registers slash commands on discord when the bot is ready
 *
 * @param ctx the dpp::cluster (bot)
 * @param slash_commands a list of commands that will be registerd
 * @return doesn't return anything
 */
static inline void register_global_slash_commands(ctx_t& ctx, const slash_commands_t& slash_commands) {
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

/**
 * @brief handle the logging events
 *
 * @param ctx the cluster that can be thought of the current bot instance
 * @return doesn't return anything but might produce console and file output
 */
static inline void handle_on_log(ctx_t& ctx) {
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

/**
 * @brief registers a handler for slash command events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered slash commands
 * @return doesn't return anything
 */
static inline void handle_global_slash_commands(ctx_t& ctx, const slash_commands_t& slash_commands) {
	ctx.on_slashcommand([&slash_commands](const dpp::slashcommand_t& event) {
		const std::string& command_name = event.command.get_command_name();
		if (slash_commands.find(command_name) != slash_commands.end()) {
			slash_commands.at(command_name)->on_slashcommand(event);
		}
	});
}

/**
 * @brief registers a handler for message create events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered message commands
 * @return doesn't return anything
 */
static inline void handle_message_create(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx.on_message_create([&message_commands](const dpp::message_create_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_create(event);
		}
	});
}

/**
 * @brief registers a handler for message delete events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered message commands
 * @return doesn't return anything
 */
static inline void handle_message_delete(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx.on_message_delete([&message_commands](const dpp::message_delete_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete(event);
		}
	});
}

/**
 * @brief registers a handler for message delete bulk events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered message commands
 * @return doesn't return anything
 */
static inline void handle_message_delete_bulk(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx.on_message_delete_bulk([&message_commands](const dpp::message_delete_bulk_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete_bulk(event);
		}
	});
}

/**
 * @brief registers a handler for guild member add events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered member commands
 * @return doesn't return anything
 */
static inline void handle_guild_member_add(ctx_t& ctx, const member_commands_t& member_commands) {
	ctx.on_guild_member_add([&member_commands](const dpp::guild_member_add_t& event) {
		for (const auto& command : member_commands) {
			command->on_guild_member_add(event);
		}
	});
}

/**
 * @brief registers a handler for guild member remove events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered member commands
 * @return doesn't return anything
 */
static inline void handle_guild_member_remove(ctx_t& ctx, const member_commands_t& member_commands) {
	ctx.on_guild_member_remove([&member_commands](const dpp::guild_member_remove_t& event) {
		for (const auto& command : member_commands) {
			command->on_guild_member_remove(event);
		}
	});
}

/**
 * @brief registers a handler for button click events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param button_commands a list of registered button commands
 * @return doesn't return anything
 */
static inline void handle_button_click(ctx_t& ctx, const button_commands_t& button_commands) {
	(void)ctx;
	(void)button_commands;
}

/**
 * @brief registers a handler for form submit events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param form_commands a list of registered form commands
 * @return doesn't return anything
 */
static inline void handle_form_submit(ctx_t& ctx, const form_commands_t& form_commands) {
	(void)ctx;
	(void)form_commands;
}

/**
 * @brief registers a handler for reaction add events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param reaction_commands a list of registered reaction commands
 * @return doesn't return anything
 */
static inline void handle_reaction_add(ctx_t& ctx, const reaction_commands_t& reaction_commands) {
	(void)ctx;
	(void)reaction_commands;
}

/**
 * @brief registers a handler for reaction remove events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param reaction_commands a list of registered reaction commands
 * @return doesn't return anything
 */
static inline void handle_reaction_remove(ctx_t& ctx, const reaction_commands_t& reaction_commands) {
	(void)ctx;
	(void)reaction_commands;
}

/**
 * @brief registers a handler for ready events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param ready_commands a list of registered ready commands
 * @return doesn't return anything
 */
static inline void handle_ready(ctx_t& ctx, const ready_commands_t& ready_commands) {
	(void)ctx;
	(void)ready_commands;
}

/**
 * @brief registeres all callbacks and starts the bot
 *
 * @return doesn't return anything
 */
void Bot::run() {
	ctx_t ctx(Bot::ctx_token);

	// custom logger
	handle_on_log(ctx);

	// slash commands
	register_global_slash_commands(ctx, Bot::slash_commands);
	handle_global_slash_commands(ctx, Bot::slash_commands);

	// message commands
	handle_message_create(ctx, Bot::message_commands);
	handle_message_delete(ctx, Bot::message_commands);
	handle_message_delete_bulk(ctx, Bot::message_commands);

	// guild members
	handle_guild_member_add(ctx, Bot::member_commands);
	handle_guild_member_remove(ctx, Bot::member_commands);

	// button clicks
	handle_button_click(ctx, Bot::button_commands);

	// form submits
	handle_form_submit(ctx, Bot::form_commands);

	// reaction commands
	handle_reaction_add(ctx, Bot::reaction_commands);
	handle_reaction_remove(ctx, Bot::reaction_commands);

	// when bot is ready
	handle_ready(ctx, Bot::ready_commands);

	ctx.start(dpp::st_wait);
	return;
}