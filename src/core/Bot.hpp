#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "interfaces/IButtonCommand.hpp"
#include "interfaces/IFormCommand.hpp"
#include "interfaces/IGlobalSlashCommand.hpp"
#include "interfaces/IMemberCommand.hpp"
#include "interfaces/IMessageCommand.hpp"
#include "interfaces/IReactionCommand.hpp"
#include "interfaces/IReady.hpp"

using button_commands_t = std::map<std::string, std::shared_ptr<IButtonCommand>>;
using form_commands_t = std::map<std::string, std::shared_ptr<IFormCommand>>;
using slash_commands_t = std::map<std::string, std::shared_ptr<IGlobalSlashCommand>>;

using member_commands_t = std::vector<std::shared_ptr<IMemberCommand>>;
using message_commands_t = std::vector<std::shared_ptr<IMessageCommand>>;
using reaction_commands_t = std::vector<std::shared_ptr<IReactionCommand>>;
using ready_commands_t = std::vector<std::shared_ptr<IReady>>;

class Bot {
  private:
	static dpp::cluster ctx;
	static button_commands_t button_commands;
	static form_commands_t form_commands;
	static slash_commands_t slash_commands;
	static member_commands_t member_commands;
	static message_commands_t message_commands;
	static reaction_commands_t reaction_commands;
	static ready_commands_t ready_commands;

  public:
	static void set_token(const std::string& token);

	static void add_button_command(const std::string& command_name,
								   const std::shared_ptr<IButtonCommand>& button_command);
	static void add_form_command(const std::string& command_name, const std::shared_ptr<IFormCommand>& form_command);
	static void add_slash_command(const std::string& command_name,
								  const std::shared_ptr<IGlobalSlashCommand>& slash_command);

	static void add_member_command(const std::shared_ptr<IMemberCommand>& button_command);
	static void add_message_command(const std::shared_ptr<IMessageCommand>& button_command);
	static void add_reaction_command(const std::shared_ptr<IReactionCommand>& button_command);
	static void add_ready_command(const std::shared_ptr<IReady>& ready_command);

	static void run();
};
