#include "language_server.hpp"

auto str{ std::string("nope") };

auto LanguageServer::get(size_t guild_id, const std::string& string_id) -> const std::string& {
    return str;
}