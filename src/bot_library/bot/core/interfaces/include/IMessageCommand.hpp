#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IMessageCommand {
  public:
	virtual auto on_message_create(const dpp::message_create_t& event) -> void = 0;

	virtual auto on_message_delete(const dpp::message_delete_t&) -> void = 0;

	virtual auto on_message_delete_bulk(const dpp::message_delete_bulk_t&) -> void = 0;
};