#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

//-----------------------------------------------------
//
//-----------------------------------------------------
class LeaderboardHandler : public Poco::Net::HTTPRequestHandler {
  public:
	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;
};