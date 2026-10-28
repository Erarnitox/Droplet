#pragma once

class Application {
  public:
	/// @return process exit code (0 on clean shutdown)
	[[nodiscard]] int run(bool is_test);
};
