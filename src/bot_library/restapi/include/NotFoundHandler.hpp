#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

#include <AuthUtils.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
class NotFoundHandler : public Poco::Net::HTTPRequestHandler {
  public:
	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;
};