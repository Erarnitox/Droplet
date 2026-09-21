#pragma once

#include "Poco/Net/HTTPRequestHandlerFactory.h"

class DatabaseExecutor;

class RestApiRequestHandlerFactory final : public Poco::Net::HTTPRequestHandlerFactory {
  public:
	explicit RestApiRequestHandlerFactory(DatabaseExecutor& db) : db_(db) {
	}

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;

  private:
	DatabaseExecutor& db_;
};
