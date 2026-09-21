/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IMessageCommand.hpp>

struct AppContext;

class LearnCppTutorialHint : public IMessageCommand {
  public:
	explicit LearnCppTutorialHint(AppContext& ctx);

	void on_message_create(const dpp::message_create_t& event) override;

  private:
	dpp::cluster& discord_;
};
