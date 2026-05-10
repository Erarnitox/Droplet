/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "BotSlashCommandRegistrar.hpp"

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/once.h>

#include <fstream>
#include <string>

namespace {

constexpr char HASH_FILE[]{"command_hash"};

[[nodiscard]] size_t get_prior_hash() {
	std::ifstream hash_file;
	hash_file.open(HASH_FILE, std::ios::in | std::ios::binary);

	size_t hash{0ull};
	if (hash_file.is_open()) {
		hash_file.read(reinterpret_cast<char*>(&hash), sizeof(hash));
	}

	hash_file.close();
	return hash;
}

[[nodiscard]] size_t get_current_hash(const slash_commands_t& slash_commands) {
	size_t sum{0};
	for (const auto& slash_command : slash_commands) {
		sum ^= std::hash<std::string>{}(slash_command.first);
		for (const auto& option : slash_command.second->command_options) {
			sum ^= std::hash<std::string>{}(option.name);
		}
	}
	return sum;
}

void save_hash_to_file(size_t hash) {
	std::ofstream hash_file;
	hash_file.open(HASH_FILE, std::ios::out | std::ios::binary | std::ios::trunc);

	if (hash_file.is_open()) {
		hash_file.write(reinterpret_cast<char*>(&hash), sizeof(hash));
	}
	hash_file.close();
}

}  // namespace

void register_global_slash_commands_on_ready(ctx_t& ctx, const slash_commands_t& slash_commands) {
	ctx->on_ready([&ctx, &slash_commands](const dpp::ready_t& event) -> void {
		(void)event;

		ctx->log(dpp::ll_trace, "Registering Slash commands...");

		if (dpp::run_once<struct register_bot_commands>()) {
			const auto pHash{get_prior_hash()};
			const auto nHash{get_current_hash(slash_commands)};

			if (pHash != nHash) {
				ctx->global_bulk_command_delete();
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
}
