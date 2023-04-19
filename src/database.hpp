#pragma once

#include <cstddef>
#include <string>
#include <pqxx/pqxx>

class Database {
private:
    bool is_connected;
    pqxx::connection conn;
    
public:
    Database (
        const std::string& db_name,
        const std::string& user, 
        const std::string& password,
        const std::string& host,
        const std::string& port
    );

    Database (const std::string& connection_string);

    ~Database();

    [[nodiscard]]
    auto get_challenge_role_data(size_t message_id) -> std::pair<size_t, std::string>;
    auto insert_challenge_role_data(size_t role_id, size_t guild_id, size_t message_id, const std::string& flag) -> void;

    [[nodiscard]]
    auto get_reaction_role_data(size_t message_id, size_t reaction_emoji) -> std::pair<size_t, std::string>;
    auto insert_reaction_role_data(const std::string& role_id, size_t guild_id, const std::string& message_id, const std::string& emoji) -> void;
};