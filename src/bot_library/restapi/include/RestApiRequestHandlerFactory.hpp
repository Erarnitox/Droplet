/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include "Poco/Net/HTTPRequestHandlerFactory.h"

class RestApiRequestHandlerFactory final : public Poco::Net::HTTPRequestHandlerFactory {
  public:
	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
};
