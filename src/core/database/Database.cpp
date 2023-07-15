#include "Database.hpp"

#include <fmt/core.h>
#include <utility>
#include <string_view>
#include <map>
#include <pqxx/pqxx>

static pqxx::connection* conn{ nullptr };

auto Database::connect(
    const std::string& db_name,
    const std::string& user, 
    const std::string& password,
    const std::string& host,
    const std::string& port
) -> bool {
    Database::disconnect();
    conn = new pqxx::connection(fmt::format(
            "dbname={} user={} password={} hostaddr={} port={}",
              db_name,  user,   password,   host,       port
           )
    );

    return conn->is_open();
}

auto Database::connect(const std::string& connection_string) -> bool {
    Database::disconnect();
    conn = new pqxx::connection(connection_string);
    return conn->is_open();
}

auto Database::disconnect() noexcept -> void {
    if(conn) delete conn;
    conn = nullptr;
}

auto Database::get_reaction_role_data(size_t message_id, const std::string& reaction_emoji) noexcept -> size_t {
    static int times = 0;
    static std::string sql_string{ "SELECT role_id FROM reaction_roles WHERE message_id=$1 AND emoji=$2" };
    
    try{
        if(!conn){
            //TODO: throw an error
        }

        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, message_id, reaction_emoji);
        txn.commit();
        times = 0;

        const auto& role_id{ result.at(0, 0).get<size_t>() };
        return role_id.value();
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return 0;
        }
        Database::reconnect();
        return Database::get_reaction_role_data(message_id, reaction_emoji); 
    } catch (...) {
        fmt::print("Trying to call 'get_reaction_role_data' with message_id={} reaction_emoji={}\n", message_id, reaction_emoji);
        return 0;
    }
}

auto Database::insert_reaction_role_data(const std::string& role_id, size_t guild_id, const std::string& message_id, const std::string& emoji) noexcept -> void {
    static int times = 0;
    static std::string sql_string{ "INSERT INTO reaction_roles(role_id, guild_id, message_id, emoji) VALUES ($1, $2, $3, $4)" };

    try{
        if(!conn) return;
        if(!message_id.size()) return;
        if(Database::get_reaction_role_data(std::stoul(message_id), emoji)) return;
        
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, role_id, guild_id, message_id, emoji);
        txn.commit();
        times=0;
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return;
        }
        Database::reconnect();
        Database::insert_reaction_role_data(role_id, guild_id, message_id, emoji);
    } catch(...) {
        fmt::print("Trying to call 'insert_reaction_role_data' with role_id={} guild_id={} message_id={}\n", role_id, guild_id, message_id);
    }
}

auto Database::get_welcome_channel_id(size_t guild_id) noexcept -> size_t {
    static int times = 0;
    static std::string sql_string{ "SELECT channel_id FROM welcome_channels WHERE guild_id=$1" };
    
    try{
        if(!conn){
            //TODO: throw an error
        }
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, guild_id);
        txn.commit();
        times = 0;

        const auto& channel_id{ result.at(0, 0).get<size_t>() };
        return channel_id.value();
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return 0;
        }
        Database::reconnect();
        return Database::get_welcome_channel_id(guild_id); 
    } catch (...) {
        fmt::print("Trying to call 'get_welcome_channel_id' with guild_id={}\n", guild_id);
        return 0;
    }
}

auto Database::insert_welcome_channel_id(size_t guild_id, size_t channel_id) noexcept -> void {
    static int times = 0;
    static std::string sql_string{ "INSERT INTO welcome_channels(guild_id, channel_id) VALUES ($1, $2)" };
    try{
        if(!conn) return;
        if(!guild_id) return;
        if(Database::get_welcome_channel_id(guild_id)) return;
            
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, guild_id, channel_id);
        txn.commit();
        times=0;
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return;
        }
        Database::reconnect();
        Database::insert_welcome_channel_id(guild_id, channel_id);
    } catch(...) {
        fmt::print("Trying to call 'insert_welcome_channel_id' with guild_id={} channel_id={}\n", guild_id, channel_id);
    }
}   

auto Database::get_goodbye_channel_id(size_t guild_id) noexcept -> size_t {
    static int times = 0;
    static std::string sql_string{ "SELECT channel_id FROM goodbye_channels WHERE guild_id=$1" };
    
    try{
        if(!conn){
            //TODO: throw an error
        }
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, guild_id);
        txn.commit();
        times = 0;

        const auto& channel_id{ result.at(0, 0).get<size_t>() };
        return channel_id.value();
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return 0;
        }
        Database::reconnect();
        return Database::get_goodbye_channel_id(guild_id); 
    } catch (...) {
        fmt::print("Trying to call 'get_goodbye_channel_id' with guild_id={}\n", guild_id);
        return 0;
    }
}

auto Database::insert_goodbye_channel_id(size_t guild_id, size_t channel_id) noexcept -> void {
    static int times = 0;
    static std::string sql_string{ "INSERT INTO goodbye_channels(guild_id, channel_id) VALUES ($1, $2)" };
    try{
        if(!conn) return;
        if(!guild_id) return;
        if(Database::get_goodbye_channel_id(guild_id)) return;
            
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, guild_id, channel_id);
        txn.commit();
        times=0;
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return;
        }
        Database::reconnect();
        Database::insert_goodbye_channel_id(guild_id, channel_id);
    } catch(...) {
        fmt::print("Trying to call 'insert_goodbye_channel_id' with guild_id={} channel_id={}\n", guild_id, channel_id);
    }
} 

auto Database::get_log_channel_id(size_t guild_id) noexcept -> size_t {
    static int times = 0;
    static std::string sql_string{ "SELECT channel_id FROM log_channels WHERE guild_id=$1" };
    
    try{
        if(!conn){
            //TODO: throw an error
        }

        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, guild_id);
        txn.commit();
        times = 0;

        const auto& channel_id{ result.at(0, 0).get<size_t>() };
        return channel_id.value();
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return 0;
        }
        Database::reconnect();
        return Database::get_log_channel_id(guild_id); 
    } catch (...) {
        fmt::print("Trying to call 'get_log_channel_id' with guild_id={}\n", guild_id);
        return 0;
    }
}

auto Database::insert_log_channel_id(size_t guild_id, size_t channel_id) noexcept -> void {
    static int times = 0;
    static std::string sql_string{ "INSERT INTO log_channels(guild_id, channel_id) VALUES ($1, $2)" };
    try{
        if(!conn) return;
        if(!guild_id) return;
        if(Database::get_log_channel_id(guild_id)) return;
        
        pqxx::work txn(*conn);
        pqxx::result result = txn.exec_params(sql_string, guild_id, channel_id);
        txn.commit();
        times=0;
    } catch(const pqxx::broken_connection& e){
        ++times;
        if(times > 10){
            times = 0;
            return;
        }
        Database::reconnect();
        Database::insert_log_channel_id(guild_id, channel_id);
    } catch(...) {
        fmt::print("Trying to call 'insert_welcome_channel_id' with guild_id={} channel_id={}\n", guild_id, channel_id);
    }
} 

auto Database::hasConnection() noexcept -> bool {
    return conn->is_open();
}

auto Database::reconnect() noexcept -> void {
    static int times = 0;
    try {
        ++times;
        bool connected{ conn && conn->is_open() };
        if(!connected) {
            //conn->activate();
            connected = Database::connect(conn->connection_string());
        }
        if(connected) times = 0;
    } catch(const pqxx::broken_connection & e) {
        if(times > 10) {
            times = 0;
            return;
        }
        Database::reconnect();
    } catch(...){
        fmt::print("Error: reconnecting failed...\n");
    }
}

auto Database::execQuery(const std::string& query, const std::vector<std::type_index>& types, std::vector<void*>& args) noexcept -> void {
    static int times = 0;
    try{
        pqxx::work txn(*conn);
        
        // convert all values to strings and store them in paramValues
        std::vector<std::string> paramValues;
        for(size_t i{ 0 }; i < args.size(); ++i) {
            if(types[i] == typeid(size_t)) paramValues.push_back(std::to_string(*(size_t*)args[i]));
            else if(types[i] == typeid(std::string)) paramValues.push_back(*(std::string*)args[i]);
            else if(types[i] == typeid(int)) paramValues.push_back(std::to_string(*(int*)args[i]));
            else if(types[i] == typeid(bool)) paramValues.push_back(std::to_string(*(bool*)args[i]));
            else if(types[i] == typeid(float)) paramValues.push_back(std::to_string(*(float*)args[i]));
        }

        // perform the database transaction
        pqxx::result result = txn.exec_params(query, paramValues);
        txn.commit();

        // convert the results into the right datatype and save them
        for(size_t i{ 0 }; i < args.size(); ++i) {
            if(types[i] == typeid(size_t)) *((size_t*)args[i]) = result.at(0, i).get<size_t>().value();
            else if(types[i] == typeid(std::string)) *((std::string*)args[i]) = result.at(0, i).get<std::string>().value();
            else if(types[i] == typeid(int)) *((int*)args[i]) = result.at(0, i).get<int>().value();
            else if(types[i] == typeid(bool)) *((bool*)args[i]) = result.at(0, i).get<bool>().value();
            else if(types[i] == typeid(float)) *((float*)args[i]) = result.at(0, i).get<float>().value();
        }

        times=0;
        return;
    } catch(const pqxx::broken_connection& e) {
        ++times;
        if(times > 10){
            times = 0;
            return;
        }
        Database::reconnect();
        execQuery(query, types, args);
    } catch (...) {
        fmt::print("Error: Failed to insert Into Database!\n");
    }
}