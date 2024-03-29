#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IMessageCommand {
  public:
	virtual void on_message_create(const dpp::message_create_t& event) = 0;

	virtual void on_message_delete(const dpp::message_delete_t& event) = 0;

	virtual void on_message_delete_bulk(const dpp::message_delete_bulk_t& event) = 0;
};