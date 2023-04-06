#pragma once
#include <string>

class LanguageServer {

    public:
    static const std::string& get(size_t guild_id, const std::string& string_id);
};