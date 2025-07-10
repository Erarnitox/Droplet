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

#pragma once

#include <Core.hpp>
#include <IButtonCommand.hpp>
#include <IFormCommand.hpp>
#include <IGlobalSlashCommand.hpp>
#include <IMemberCommand.hpp>
#include <IMessageCommand.hpp>
#include <IReactionCommand.hpp>
#include <IReady.hpp>
#include <memory>
#include <string>
#include <vector>

using ctx_t = std::unique_ptr<dpp::cluster>;
using slash_commands_t = std::map<std::string, std::shared_ptr<IGlobalSlashCommand>>;
using button_commands_t = std::vector<std::shared_ptr<IButtonCommand>>;
using form_commands_t = std::vector<std::shared_ptr<IFormCommand>>;
using member_commands_t = std::vector<std::shared_ptr<IMemberCommand>>;
using message_commands_t = std::vector<std::shared_ptr<IMessageCommand>>;
using reaction_commands_t = std::vector<std::shared_ptr<IReactionCommand>>;
using ready_commands_t = std::vector<std::shared_ptr<IReady>>;

class Bot {
  public:
	static ctx_t ctx;
	static button_commands_t button_commands;
	static form_commands_t form_commands;
	static slash_commands_t slash_commands;
	static member_commands_t member_commands;
	static message_commands_t message_commands;
	static reaction_commands_t reaction_commands;
	static ready_commands_t ready_commands;

  public:
	static void init(const std::string& token);
	static void add_slash_command(const std::shared_ptr<IGlobalSlashCommand>& slash_command);
	static void add_button_command(const std::shared_ptr<IButtonCommand>& button_command);
	static void add_form_command(const std::shared_ptr<IFormCommand>& form_command);
	static void add_member_command(const std::shared_ptr<IMemberCommand>& button_command);
	static void add_message_command(const std::shared_ptr<IMessageCommand>& button_command);
	static void add_reaction_command(const std::shared_ptr<IReactionCommand>& button_command);
	static void add_ready_command(const std::shared_ptr<IReady>& ready_command);
	static void run();
};
