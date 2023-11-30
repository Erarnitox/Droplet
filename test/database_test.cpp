#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include <Database.hpp>

TEST_CASE("DatabaseTest", "[database]") {
    //REQUIRE_FALSE(Database::connect("dbname=database user=user password=123456 hostaddr=192.168.50.50 port=2332"));

    Database::disconnect();
    REQUIRE_FALSE(Database::hasConnection());
}