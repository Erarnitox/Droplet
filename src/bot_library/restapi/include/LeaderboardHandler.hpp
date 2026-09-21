#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

class DatabaseExecutor;

class LeaderboardHandler : public Poco::Net::HTTPRequestHandler {
  public:
	explicit LeaderboardHandler(DatabaseExecutor& db) : db_(db) {
	}
	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;

  private:
	DatabaseExecutor& db_;
};
