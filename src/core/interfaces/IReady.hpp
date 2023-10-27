#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>

class IReady {
  public:
	virtual auto on_ready(const dpp::ready_t &event) -> void = 0;
};