#pragma once

#include <string>

class DatabaseExecutor;

class RestApi {
  public:
	const static ushort port;
	const static bool ssl_enabled;
	static void start(DatabaseExecutor& db);
};
