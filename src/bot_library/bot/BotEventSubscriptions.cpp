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

void handle_on_log(ctx_t& ctx) {
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

void handle_global_slash_commands(ctx_t& ctx, const slash_commands_t& slash_commands) {
	ctx->on_slashcommand([&slash_commands](const dpp::slashcommand_t& event) {
		const std::string& command_name = event.command.get_command_name();
		if (slash_commands.find(command_name) != slash_commands.end()) {
			slash_commands.at(command_name)->on_slashcommand(event);
		}
	});
}

void handle_message_create(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx->on_message_create([&message_commands](const dpp::message_create_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_create(event);
		}
	});
}

void handle_message_delete(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx->on_message_delete([&message_commands](const dpp::message_delete_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete(event);
		}
	});
}

void handle_message_delete_bulk(ctx_t& ctx, const message_commands_t& message_commands) {
	ctx->on_message_delete_bulk([&message_commands](const dpp::message_delete_bulk_t& event) {
		for (const auto& command : message_commands) {
			command->on_message_delete_bulk(event);
		}
	});
}

void handle_guild_member_add(ctx_t& ctx, const member_commands_t& member_commands) {
	ctx->on_guild_member_add([&member_commands](const dpp::guild_member_add_t& event) {
		for (const auto& command : member_commands) {
			command->on_guild_member_add(event);
		}
	});
}

void handle_guild_member_remove(ctx_t& ctx, const member_commands_t& member_commands) {
	ctx->on_guild_member_remove([&member_commands](const dpp::guild_member_remove_t& event) {
		for (const auto& command : member_commands) {
			command->on_guild_member_remove(event);
		}
	});
}

void handle_button_click(ctx_t& ctx, const button_commands_t& button_commands) {
	ctx->on_button_click([&button_commands](const dpp::button_click_t& event) {
		for (const auto& command : button_commands) {
			command->on_button_click(event);
		}
	});
}

void handle_form_submit(ctx_t& ctx, const form_commands_t& form_commands) {
	ctx->on_form_submit([&form_commands](const dpp::form_submit_t& event) {
		for (const auto& command : form_commands) {
			command->on_form_submit(event);
		}
	});
}

void handle_reaction_add(ctx_t& ctx, const reaction_commands_t& reaction_commands) {
	ctx->on_message_reaction_add([&reaction_commands](const dpp::message_reaction_add_t& event) {
		for (const auto& command : reaction_commands) {
			command->on_message_reaction_add(event);
		}
	});
}

void handle_reaction_remove(ctx_t& ctx, const reaction_commands_t& reaction_commands) {
	ctx->on_message_reaction_remove([&reaction_commands](const dpp::message_reaction_remove_t& event) {
		for (const auto& command : reaction_commands) {
			command->on_message_reaction_remove(event);
		}
	});
}

void handle_ready(ctx_t& ctx, const ready_commands_t& ready_commands) {
	ctx->on_ready([&ready_commands](const dpp::ready_t& event) {
		for (const auto& command : ready_commands) {
			command->on_ready(event);
		}
	});
}

}  // namespace

void subscribe_bot_event_handlers(ctx_t& ctx) {
	handle_on_log(ctx);

	register_global_slash_commands_on_ready(ctx, Bot::slash_commands);
	handle_global_slash_commands(ctx, Bot::slash_commands);

	handle_message_create(ctx, Bot::message_commands);
	handle_message_delete(ctx, Bot::message_commands);
	handle_message_delete_bulk(ctx, Bot::message_commands);

	handle_guild_member_add(ctx, Bot::member_commands);
	handle_guild_member_remove(ctx, Bot::member_commands);

	handle_button_click(ctx, Bot::button_commands);

	handle_form_submit(ctx, Bot::form_commands);

	handle_reaction_add(ctx, Bot::reaction_commands);
	handle_reaction_remove(ctx, Bot::reaction_commands);

	handle_ready(ctx, Bot::ready_commands);
}
