#pragma once

#include <string>

class Database {
public:
    static
    auto connect(
        const std::string& db_name,
        const std::string& user, 
        const std::string& password,
        const std::string& host,
        const std::string& port
    ) -> bool;

    static
    auto disconnect() -> void;
};