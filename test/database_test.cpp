#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include <ConnectionPool.hpp>
#include <Database.hpp>

TEST_CASE("DatabaseTest", "[database]") {
	ConnectionPool pool;
	REQUIRE_FALSE(pool.has_connection());
	DatabaseExecutor executor{pool};
	REQUIRE_FALSE(executor.hasConnection());
}
