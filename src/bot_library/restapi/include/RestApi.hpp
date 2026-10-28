#pragma once

#include <string>

class DatabaseExecutor;

class RestApi {
  public:
	const static unsigned short port;
	const static bool ssl_enabled;
	/// Runs the REST server until it is asked to terminate.
	/// @return 0 on a requested shutdown, non-zero if the server could not run / stopped abnormally
	[[nodiscard]] static int start(DatabaseExecutor& db);
};
