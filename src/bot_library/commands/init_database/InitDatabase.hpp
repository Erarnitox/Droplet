#pragma once

#include <Bot.hpp>

class InitDatabase : public IReady {
  public:
	void on_ready(const dpp::ready_t& event) override;
};
