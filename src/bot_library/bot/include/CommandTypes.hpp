#pragma once

#include <IButtonCommand.hpp>
#include <IFormCommand.hpp>
#include <IGlobalSlashCommand.hpp>
#include <IMessageCommand.hpp>
#include <IReactionCommand.hpp>
#include <IReady.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

using ctx_t = std::unique_ptr<dpp::cluster>;
using slash_commands_t = std::map<std::string, std::shared_ptr<IGlobalSlashCommand>>;
using button_commands_t = std::vector<std::shared_ptr<IButtonCommand>>;
using form_commands_t = std::vector<std::shared_ptr<IFormCommand>>;
using message_commands_t = std::vector<std::shared_ptr<IMessageCommand>>;
using reaction_commands_t = std::vector<std::shared_ptr<IReactionCommand>>;
using ready_commands_t = std::vector<std::shared_ptr<IReady>>;
