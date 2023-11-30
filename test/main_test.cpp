#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include <BotLibrary.hpp>

TEST_CASE("BotStartup", "[bot]") {
    std::cout << "Starting Bot..." << std::endl;
    start_bot(true);
    std::cout << "Test was run!" << std::endl;

    // if no exception was thrown
    // and execution goes here, everything went fine
    CHECK(true);
}