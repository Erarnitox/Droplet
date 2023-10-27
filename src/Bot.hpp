#pragma once

#include "core/interfaces/IFormCommand.hpp"
#include "core/interfaces/IButtonCommand.hpp"
#include "core/interfaces/IMemberCommand.hpp"
#include "core/interfaces/IGlobalSlashCommand.hpp"
#include "core/interfaces/IReactionCommand.hpp"
#include "core/interfaces/IMessageCommand.hpp"

class Bot {
private:
    static dpp::cluster ctx;
    static std::map<std::string, std::shared_ptr<IButtonCommand>> button_commands;
    static std::map<std::string, std::shared_ptr<IFormCommand>> form_commands;
    static std::map<std::string, std::shared_ptr<IGlobalSlashCommand>> slash_commands;
    static std::map<std::string, std::shared_ptr<IMemberCommand>> member_commands;
	static std::map<std::string, std::shared_ptr<IMessageCommand>> message_commands;
    static std::map<std::string, std::shared_ptr<IReactionCommand>> reaction_commands;

public:
    static void set_token(const std::string& token);

    //static void add_button_command(const IButtonCommand& );

    void run();
};
