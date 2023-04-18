#include "database.hpp"

#include <fmt/core.h>
#include <utility>

Database::Database(
    const std::string& db_name,
    const std::string& user, 
    const std::string& password,
    const std::string& host,
    const std::string& port
):
db_name{ db_name }, user{ user }, password{ password }, host{ host }, port{ port },
conn{ 
        pqxx::connection(fmt::format(
            "dbname={} user={} password={} hostaddr={} port={}",
             db_name,  user,   password,   host,       port
        ))
    }

{
    if(!conn.is_open()) this->is_connected = false;
    this->is_connected = true;
}

auto Database::disconnect() -> void {
    //conn.disconnect();
}

auto Database::get_challenge_role_data(size_t message_id) -> std::pair<size_t, std::string> {
    static std::string sql_string{ "SELECT role_id, flag FROM challenge_roles WHERE message_id=$1" };
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params(sql_string, 0 /*message_id*/);
    txn.commit();

    const auto& role_id{ result.at(0, 0).get<size_t>().value() };
    const auto& flag{ result.at(0, 1).get<std::string>().value() };
    return { role_id, flag };
}

auto Database::insert_challenge_role_data(size_t role_id, size_t guild_id, size_t message_id, std::string flag) -> void {
    static std::string sql_string{ "INSERT INTO challenge_roles(role_id, guild_id, message_id, flag) VALUES ($1, $2, $3, $4)" };
    pqxx::work txn(conn);
    pqxx::result result = txn.exec_params(sql_string, role_id, guild_id, message_id, flag);
    txn.commit();
}