/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <Bot.hpp>

/**
 * Listens to guild messages and replies when someone appears to ask how to learn C++ / find tutorials.
 */
class LearnCppTutorialHint final : public IMessageCommand {
  public:
	LearnCppTutorialHint();

	void on_message_create(const dpp::message_create_t& event) override;

	void on_message_delete(const dpp::message_delete_t& event) override;

	void on_message_delete_bulk(const dpp::message_delete_bulk_t& event) override;
};
