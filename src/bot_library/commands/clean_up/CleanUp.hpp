/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IMessageCommand.hpp>

struct AppContext;
class DatabaseExecutor;

class CleanUp : public IMessageCommand {
  public:
	explicit CleanUp(AppContext& ctx);

	void on_message_delete(const dpp::message_delete_t& event) override;
	void on_message_delete_bulk(const dpp::message_delete_bulk_t& event) override;

  private:
	void clean_up_message(const dpp::snowflake& message_id);
	DatabaseExecutor& db_;
};
