#include "Bot.hpp"

Bot::Bot(const std::string& token) : ctx(token) {
        // Initialize and register commands
        message_commands["ping"] = std::make_shared<PongCommand>();
    }

void Bot::run() {
    ctx.on_message_create([this](const dpp::message_create_t& event) {
        const std::string& command_name = event.msg.content;
        if (message_commands.find(command_name) != message_commands.end()) {
            message_commands[command_name]->on_message_create(event);
        }
    });

    ctx.start(dpp::st_wait);
	return;
}