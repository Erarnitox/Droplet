#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <iostream>

#include <bot.hpp>

TEST_CASE("BotStartup", "[bot]") {
    // do things
    std::cout << "Test was run!" << std::endl;
    CHECK(true);
}