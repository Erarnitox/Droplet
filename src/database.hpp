#pragma once

#include <string>

class Database {
public:

    [[nodiscard("Check if the Connection was established!")]]
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