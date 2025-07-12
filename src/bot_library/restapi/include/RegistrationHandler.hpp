/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

//-----------------------------------------------------
//
//-----------------------------------------------------
class RegistrationHandler : public Poco::Net::HTTPRequestHandler {
  public:
	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;
};