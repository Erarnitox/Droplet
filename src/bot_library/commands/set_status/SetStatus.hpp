/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <IReady.hpp>

struct AppContext;

class SetStatus : public IReady {
  public:
	explicit SetStatus(AppContext& ctx);

	void on_ready(const dpp::ready_t& event) override;

  private:
	dpp::cluster& discord_;
};
