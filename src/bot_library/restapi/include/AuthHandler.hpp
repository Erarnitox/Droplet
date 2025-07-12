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

#include <AuthUtils.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
class AuthHandler : public Poco::Net::HTTPRequestHandler {
  public:
	AuthHandler(const std::string& secret, const std::string& issuer) : _secret(secret), _issuer(issuer) {
	}
	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;

  private:
	std::string _secret, _issuer;
};