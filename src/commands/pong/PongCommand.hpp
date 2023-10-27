#pragma once

#include "../../core/Core.hpp"

class PongCommand : public IMessageCommand {
	std::string command_name;

  public:
	void on_message_create(const dpp::message_create_t& event) override;

	void on_message_delete(const dpp::message_delete_t&) override;

	void on_message_delete_bulk(const dpp::message_delete_bulk_t&) override;
};
