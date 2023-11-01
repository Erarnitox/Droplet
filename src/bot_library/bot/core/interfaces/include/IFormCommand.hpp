#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IFormCommand {
  public:
	virtual auto on_form_submit(const dpp::form_submit_t& event) -> void = 0;
};