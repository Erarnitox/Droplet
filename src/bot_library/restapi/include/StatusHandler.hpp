/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include "Poco/Net/HTTPRequestHandler.h"

class StatusHandler final : public Poco::Net::HTTPRequestHandler {
  public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};
