#pragma once

#include "Poco/Util/ServerApplication.h"
#include "RestApi.hpp"

class DatabaseExecutor;

class RestApiServerApplication final : public Poco::Util::ServerApplication {
  public:
	explicit RestApiServerApplication(DatabaseExecutor& db) : db_(db) {
	}

  protected:
	int main(const std::vector<std::string>& args) override;

  private:
	DatabaseExecutor& db_;
};
