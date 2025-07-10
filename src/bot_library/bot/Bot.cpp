/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class handles the execution of the
 *	bot and delegats events to registerd commands.
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include "Bot.hpp"

#include <appcommand.h>
#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>
#include <intents.h>

#include <fstream>
#include <memory>

// initialize static members
ctx_t Bot::ctx;
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
 */
void Bot::init(const std::string& token) {
	const auto intents{dpp::i_default_intents | dpp::i_message_content | dpp::i_guild_messages};
	Bot::ctx = std::make_unique<dpp::cluster>(token, intents);

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
 * @param slash_command the command that implements the IGlobalSlashCommand interface
 */
void Bot::add_slash_command(const std::shared_ptr<IGlobalSlashCommand>& slash_command) {
	Bot::slash_commands[slash_command->command_name] = slash_command;
}

/**
 * @brief registers new button commands with the bot
 *
 * @param button_command the command that implements the IButtonCommand interface
 */
void Bot::add_button_command(const std::shared_ptr<IButtonCommand>& button_command) {
	Bot::button_commands.push_back(button_command);
}

/**
 * @brief registers new form commands with the bot
 *
 * @param form_command the command that implements the IFormCommand interface
 */
void Bot::add_form_command(const std::shared_ptr<IFormCommand>& form_command) {
	Bot::form_commands.push_back(form_command);
}

/**
 * @brief registers new member commands with the bot
 *
 * @param member_command the command that implements the IMemberCommand interface
 */
void Bot::add_member_command(const std::shared_ptr<IMemberCommand>& member_command) {
	Bot::member_commands.push_back(member_command);
}

/**
 * @brief registers new message commands with the bot
 *
 * @param message_command the command that implements the IMessageCommand interface
 */
void Bot::add_message_command(const std::shared_ptr<IMessageCommand>& message_command) {
	Bot::message_commands.push_back(message_command);
}

/**
 * @brief registers new reaction commands with the bot
 *
 * @param reaction_command the command that implements the IReactionCommand interface
 */
void Bot::add_reaction_command(const std::shared_ptr<IReactionCommand>& reaction_command) {
	Bot::reaction_commands.push_back(reaction_command);
}

/**
 * @brief registers new ready commands with the bot
 *
 * @param ready_command the command that implements the IReady interface
 */
void Bot::add_ready_command(const std::shared_ptr<IReady>& ready_command) {
	Bot::ready_commands.push_back(ready_command);
}

// file to store the command checksum in
constexpr char HASH_FILE[]{"command_hash"};

/**
 * @brief reads a checksum of the previously registered commands form a file
 *
 * @return returns the read checksum or 0 if none could be read
 */
static inline size_t get_prior_hash() {
	std::ifstream hash_file;
	hash_file.open(HASH_FILE, std::ios::in | std::ios::binary);

	size_t hash{0ull};
	if (hash_file.is_open()) {
		hash_file.read(reinterpret_cast<char*>(&hash), sizeof(hash));
	}

	hash_file.close();
	return hash;
}

/**
 * @brief calculates a simple checksum of the current slash commands
 *
 * @param slash_commands a list of slash commands
 * @return returns the calculated checksum for the provided list of slash commands
 */
static inline size_t get_current_hash(const slash_commands_t& slash_commands) {
	size_t sum{0};

	// calculate a simple checksum
	for (const auto& slash_command : slash_commands) {
		sum ^= std::hash<std::string>{}(slash_command.first);
		for (const auto& option : slash_command.second->command_options) {
			sum ^= std::hash<std::string>{}(option.name);
		}
	}

	return sum;
}

/**
 * @brief saves a checksum to a file on disc
 *
 * @param hash the hash to be saved to file
 */
static inline void save_hash_to_file(size_t hash) {
	std::ofstream hash_file;
	hash_file.open(HASH_FILE, std::ios::out | std::ios::binary | std::ios::trunc);

	if (hash_file.is_open()) {
		hash_file.write(reinterpret_cast<char*>(&hash), sizeof(hash));
	}
	hash_file.close();
	return;
}

/**
 * @brief registers slash commands on discord when the bot is ready
 *
 * @param ctx the dpp::cluster (bot)
 * @param slash_commands a list of commands that will be registerd
 */
static inline void register_global_slash_commands(ctx_t& ctx, const slash_commands_t& slash_commands) {
	ctx->on_ready([&ctx, &slash_commands](const dpp::ready_t& event) -> void {
		(void)event;

		ctx->log(dpp::ll_trace, "Registering Slash commands...");

		if (dpp::run_once<struct register_bot_commands>()) {
			// get the hash of previous registered commands
			const auto pHash{get_prior_hash()};
			const auto nHash{get_current_hash(slash_commands)};

			if (pHash != nHash) {
				ctx->global_bulk_command_delete();	// clear out the old commands
			};

			for (const auto& slash_command : slash_commands) {
				dpp::slashcommand tmp_command(
					slash_command.first, slash_command.second->command_description, ctx->me.id);

				for (const auto& option : slash_command.second->command_options) {
					tmp_command.add_option(option);
				}

				ctx->global_command_create(tmp_command);
			}
			save_hash_to_file(nHash);
		}
	});

	return;
}

/**
 * @brief handle the logging events
 *
 * @param ctx the cluster that can be thought of the current bot instance
 */
static inline void handle_on_log(ctx_t& ctx) {
	//-----------------------------------------------------------------------------
	// Functionality for Logging
	//-----------------------------------------------------------------------------

	ctx->on_log([](const dpp::log_t& event) {
		if (event.severity == dpp::ll_trace) {
			std::puts(
				std::format("[TRACE]\t\t({})\t\t\"{}\"", dpp::utility::current_date_time(), event.message).c_str());
		} else if (event.severity == dpp::ll_debug) {
			std::puts(
				std::format("[DEBUG]\t\t({})\t\t\"{}\"", dpp::utility::current_date_time(), event.message).c_str());
		} else if (event.severity == dpp::ll_info) {
			std::puts(
				std::format("[INFO]\t\t({})\t\t\"{}\"", dpp::utility::current_date_time(), event.message).c_str());
		} else if (event.severity == dpp::ll_warning) {
			std::puts(
				std::format("[WARNING]\t({})\t\t\"{}\"", dpp::utility::current_date_time(), event.message).c_str());
		} else if (event.severity == dpp::ll_error) {
			std::puts(
				std::format("[ERROR]\t\t({})\t\t\"{}\"", dpp::utility::current_date_time(), event.message).c_str());
		} else if (event.severity == dpp::ll_critical) {
			std::puts(
				std::format("[CRITICAL]\t({})\t\t\"{}\"", dpp::utility::current_date_time(), event.message).c_str());
		}
	});
}

/**
 * @brief registers a handler for slash command events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param slash_commands a list of registered slash commands
 */
static inline void handle_global_slash_commands(ctx_t& ctx, const slash_commands_t& slash_commands) {
	ctx->on_slashcommand([&slash_commands](const dpp::slashcommand_t& event) {
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
 */
static inline void handle_message_create(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx->on_message_create([&message_commands](const dpp::message_create_t& event) {
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
 */
static inline void handle_message_delete(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx->on_message_delete([&message_commands](const dpp::message_delete_t& event) {
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
 */
static inline void handle_message_delete_bulk(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx->on_message_delete_bulk([&message_commands](const dpp::message_delete_bulk_t& event) {
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
 */
static inline void handle_guild_member_add(ctx_t& ctx, const member_commands_t& member_commands) {
	ctx->on_guild_member_add([&member_commands](const dpp::guild_member_add_t& event) {
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
 */
static inline void handle_guild_member_remove(ctx_t& ctx, const member_commands_t& member_commands) {
	ctx->on_guild_member_remove([&member_commands](const dpp::guild_member_remove_t& event) {
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
 */
static inline void handle_button_click(ctx_t& ctx, const button_commands_t& button_commands) {
	ctx->on_button_click([&button_commands](const dpp::button_click_t& event) {
		for (const auto& command : button_commands) {
			command->on_button_click(event);
		}
	});
}

/**
 * @brief registers a handler for form submit events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param form_commands a list of registered form commands
 */
static inline void handle_form_submit(ctx_t& ctx, const form_commands_t& form_commands) {
	ctx->on_form_submit([&form_commands](const dpp::form_submit_t& event) {
		for (const auto& command : form_commands) {
			command->on_form_submit(event);
		}
	});
}

/**
 * @brief registers a handler for reaction add events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param reaction_commands a list of registered reaction commands
 */
static inline void handle_reaction_add(ctx_t& ctx, const reaction_commands_t& reaction_commands) {
	ctx->on_message_reaction_add([&reaction_commands](const dpp::message_reaction_add_t& event) {
		for (const auto& command : reaction_commands) {
			command->on_message_reaction_add(event);
		}
	});
}

/**
 * @brief registers a handler for reaction remove events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param reaction_commands a list of registered reaction commands
 */
static inline void handle_reaction_remove(ctx_t& ctx, const reaction_commands_t& reaction_commands) {
	ctx->on_message_reaction_remove([&reaction_commands](const dpp::message_reaction_remove_t& event) {
		for (const auto& command : reaction_commands) {
			command->on_message_reaction_remove(event);
		}
	});
}

/**
 * @brief registers a handler for ready events
 *
 * @param ctx the cluster that can be thought of as the current bot instance
 * @param ready_commands a list of registered ready commands
 */
static inline void handle_ready(ctx_t& ctx, const ready_commands_t& ready_commands) {
	ctx->on_ready([&ready_commands](const dpp::ready_t& event) {
		for (const auto& command : ready_commands) {
			command->on_ready(event);
		}
	});
}

/**
 * @brief registeres all callbacks and starts the bot
 *
 */
void Bot::run() {
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

	Bot::ctx->start(dpp::st_wait);
	return;
}