#pragma once

#include <Bot.hpp>

class SetStatus : public IReady {
  public:
	virtual void on_ready(const dpp::ready_t& event) override;
};