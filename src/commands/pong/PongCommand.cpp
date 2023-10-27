#include "PongCommand.hpp"

void PongCommand::on_message_create(const dpp::message_create_t& event) {
	event.reply(R"(
        ```cpp
            #include <iostream>

            int main(){
                std::cout << "Pong!" << std::endl;
                return 0;
            }
        ```
    )");
}

void PongCommand::on_message_delete(const dpp::message_delete_t& event) {
	(void)event;
}

void PongCommand::on_message_delete_bulk(const dpp::message_delete_bulk_t& event) {
	(void)event;
}