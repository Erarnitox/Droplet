#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IFormCommand {
  public:
	virtual ~IFormCommand() = default;

	virtual void on_form_submit(const dpp::form_submit_t& event) = 0;
};
