#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IButtonCommand {
  public:
	virtual auto on_button_click(const dpp::button_click_t &event) -> void = 0;
};