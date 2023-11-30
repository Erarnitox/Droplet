#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include <Core.hpp>

TEST_CASE("BotLibraryCoreTest", "[core]") {
    dpp::guild_member member;
    
    REQUIRE(!Core::is_admin(member));

    const std::string role_mention("<@&949343527651921920>");
    std::string role_id{ "949343527651921920" };
    REQUIRE(Core::get_role_id(role_mention) == role_id);

    const std::string channel_mention("<#459859776352944129>");
    std::string channel_id{ "459859776352944129" };
    REQUIRE(Core::get_channel_id(channel_mention) == channel_id);
    
    REQUIRE(true);
}