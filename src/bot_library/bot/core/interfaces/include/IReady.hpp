#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IReady {
  public:
	virtual void on_ready(const dpp::ready_t& event) = 0;
};