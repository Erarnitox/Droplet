#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IButtonCommand {
  public:
	virtual void on_button_click(const dpp::button_click_t& event) = 0;
};