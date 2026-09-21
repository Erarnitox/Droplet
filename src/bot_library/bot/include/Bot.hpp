#pragma once

#include <string>

#include "CommandTypes.hpp"

class Bot {
  public:
	explicit Bot(const std::string& token);
	Bot(const Bot&) = delete;
	Bot& operator=(const Bot&) = delete;

	[[nodiscard]] dpp::cluster& discord() noexcept {
		return *ctx;
	}

	void add_slash_command(const std::shared_ptr<IGlobalSlashCommand>& slash_command);
	void add_button_command(const std::shared_ptr<IButtonCommand>& button_command);
	void add_form_command(const std::shared_ptr<IFormCommand>& form_command);
	void add_message_command(const std::shared_ptr<IMessageCommand>& message_command);
	void add_message_delete_command(const std::shared_ptr<IMessageCommand>& message_command);
	void add_reaction_command(const std::shared_ptr<IReactionCommand>& reaction_command);
	void add_ready_command(const std::shared_ptr<IReady>& ready_command);
	void run();
	void shutdown() noexcept;

	slash_commands_t slash_commands;
	button_commands_t button_commands;
	form_commands_t form_commands;
	message_commands_t message_commands;
	message_commands_t message_delete_commands;
	reaction_commands_t reaction_commands;
	ready_commands_t ready_commands;

  private:
	ctx_t ctx;
};
