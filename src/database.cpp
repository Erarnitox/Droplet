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

Database::Database(const std::string& connection_string):
conn{ pqxx::connection(connection_string) }
{
    if(!conn.is_open()) this->is_connected = false;
    this->is_connected = true;
}

Database::~Database() {
}

auto Database::get_challenge_role_data(size_t message_id) -> std::pair<size_t, std::string> {
    static int times = 0;
    static std::string sql_string{ "SELECT role_id, flag FROM challenge_roles WHERE message_id=$1" };
    
    try{
        pqxx::work txn(conn);
        pqxx::result result = txn.exec_params(sql_string, message_id);
        txn.commit();

        const auto& role_id{ result.at(0, 0).get<size_t>() };
        const auto& flag{ result.at(0, 1).get<std::string>() };
        times = 0;
        return { role_id.value(), flag.value() };
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            this->is_connected = false;
            return {0, 0};
        }
        this->reconnect();
        return this->get_challenge_role_data(message_id);
    }
}

auto Database::insert_challenge_role_data(size_t role_id, size_t guild_id, size_t message_id, const std::string& flag) -> void {
    static int times = 0;
    static std::string sql_string{ "INSERT INTO challenge_roles(role_id, guild_id, message_id, flag) VALUES ($1, $2, $3, $4)" };
    
    try{
        pqxx::work txn(conn);
        pqxx::result result = txn.exec_params(sql_string, role_id, guild_id, message_id, flag);
        txn.commit();
        times=0;
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            this->is_connected = false;
            return;
        }
        this->reconnect();
        this->insert_challenge_role_data(role_id, guild_id, message_id, flag);
    }
}

auto Database::get_reaction_role_data(size_t message_id, const std::string& reaction_emoji) -> size_t {
    static int times = 0;
    static std::string sql_string{ "SELECT role_id FROM reaction_roles WHERE message_id=$1 AND emoji=$2" };
    
    try{
        pqxx::work txn(conn);
        pqxx::result result = txn.exec_params(sql_string, message_id, reaction_emoji);
        txn.commit();
        times = 0;

        const auto& role_id{ result.at(0, 0).get<size_t>() };
        return role_id.value();
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            this->is_connected = false;
            return 0;
        }
        this->reconnect();
        return this->get_reaction_role_data(message_id, reaction_emoji); 
    }

}

auto Database::insert_reaction_role_data(const std::string& role_id, size_t guild_id, const std::string& message_id, const std::string& emoji) -> void {
    static int times = 0;
    static std::string sql_string{ "INSERT INTO reaction_roles(role_id, guild_id, message_id, emoji) VALUES ($1, $2, $3, $4)" };

    try{
        pqxx::work txn(conn);
        pqxx::result result = txn.exec_params(sql_string, role_id, guild_id, message_id, emoji);
        txn.commit();
        times=0;
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            this->is_connected = false;
            return;
        }
        this->reconnect();
        this->insert_reaction_role_data(role_id, guild_id, message_id, emoji);
    } 
}

auto Database::has_connection() -> bool {
    return this->is_connected && conn.is_open();
}

auto Database::reconnect() -> void {
    static int times = 0;
    try {
        times++;
        if(!conn.is_open()) {
            //conn.activate();
            conn = pqxx::connection(conn.connection_string());
        }
        times = 0;
    } catch(const pqxx::broken_connection & e) {
        if(times > 10) {
            times = 0;
            return;
        }
        this->reconnect();
    }
}