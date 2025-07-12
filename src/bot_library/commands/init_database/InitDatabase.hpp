/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#pragma once

#include <Bot.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
class InitDatabase : public IReady {
  public:
	void on_ready(const dpp::ready_t& event) override;
};
