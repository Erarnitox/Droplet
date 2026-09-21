/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "BotEventSubscriptions.hpp"

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/utility.h>

#include <cstdio>
#include <format>
#include <string>

#include "BotSlashCommandRegistrar.hpp"

namespace {

void handle_on_log(dpp::cluster& ctx) {
	ctx.on_log([](const dpp::log_t& event) {
		if (event.severity < dpp::ll_warning) {
			return;
		}
		const char* level = event.severity == dpp::ll_critical ? "CRITICAL" : "WARNING";
		if (event.severity == dpp::ll_error) {
			level = "ERROR";
		}
		std::puts(std::format("[{}]\t({})\t\"{}\"", level, dpp::utility::current_date_time(), event.message).c_str());
	});
}

void handle_global_slash_commands(dpp::cluster& ctx, const slash_commands_t& slash_commands) {
	ctx.on_slashcommand([&slash_commands](const dpp::slashcommand_t& event) {
		const std::string& command_name = event.command.get_command_name();
		if (const auto it = slash_commands.find(command_name); it != slash_commands.end()) {
			it->second->on_slashcommand(event);
		}
	});
}

void handle_message_create(dpp::cluster& ctx, const message_commands_t& message_commands) {
	ctx.on_message_create([&message_commands](const dpp::message_create_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_create(event);
		}
	});
}

void handle_message_delete(dpp::cluster& ctx, const message_commands_t& message_commands) {
	ctx.on_message_delete([&message_commands](const dpp::message_delete_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete(event);
		}
	});
}

void handle_message_delete_bulk(dpp::cluster& ctx, const message_commands_t& message_commands) {
	ctx.on_message_delete_bulk([&message_commands](const dpp::message_delete_bulk_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete_bulk(event);
		}
	});
}

void handle_button_click(dpp::cluster& ctx, const button_commands_t& button_commands) {
	ctx.on_button_click([&button_commands](const dpp::button_click_t& event) {
		for (const auto& command : button_commands) {
			command->on_button_click(event);
		}
	});
}

void handle_form_submit(dpp::cluster& ctx, const form_commands_t& form_commands) {
	ctx.on_form_submit([&form_commands](const dpp::form_submit_t& event) {
		for (const auto& command : form_commands) {
			command->on_form_submit(event);
		}
	});
}

void handle_reaction_add(dpp::cluster& ctx, const reaction_commands_t& reaction_commands) {
	ctx.on_message_reaction_add([&reaction_commands](const dpp::message_reaction_add_t& event) {
		for (const auto& command : reaction_commands) {
			command->on_message_reaction_add(event);
		}
	});
}

void handle_reaction_remove(dpp::cluster& ctx, const reaction_commands_t& reaction_commands) {
	ctx.on_message_reaction_remove([&reaction_commands](const dpp::message_reaction_remove_t& event) {
		for (const auto& command : reaction_commands) {
			command->on_message_reaction_remove(event);
		}
	});
}

void handle_ready(dpp::cluster& ctx, const ready_commands_t& ready_commands) {
	ctx.on_ready([&ready_commands](const dpp::ready_t& event) {
		for (const auto& command : ready_commands) {
			command->on_ready(event);
		}
	});
}

}  // namespace

void subscribe_bot_event_handlers(Bot& bot) {
	dpp::cluster& ctx{bot.discord()};
	handle_on_log(ctx);

	register_global_slash_commands_on_ready(ctx, bot.slash_commands);
	handle_global_slash_commands(ctx, bot.slash_commands);

	handle_message_create(ctx, bot.message_commands);
	handle_message_delete(ctx, bot.message_delete_commands);
	handle_message_delete_bulk(ctx, bot.message_delete_commands);

	handle_button_click(ctx, bot.button_commands);
	handle_form_submit(ctx, bot.form_commands);

	handle_reaction_add(ctx, bot.reaction_commands);
	handle_reaction_remove(ctx, bot.reaction_commands);

	handle_ready(ctx, bot.ready_commands);
}
