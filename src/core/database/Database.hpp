#pragma once

#include <pqxx/pqxx>

#include <cstddef>
#include <string>
#include <optional>
#include <typeindex>
#include <vector>

class Database {
    
public:
    Database() = delete;
    
    [[nodiscard]]
    static auto connect (
        const std::string& db_name,
        const std::string& user, 
        const std::string& password,
        const std::string& host,
        const std::string& port
    ) -> bool;

    [[nodiscard]]
    static auto connect(const std::string& connection_string) -> bool;
    
    static auto hasConnection() noexcept -> bool;
    static auto reconnect() noexcept -> void;
    static auto disconnect() noexcept -> void;
    static auto getConnection() noexcept -> pqxx::connection*;

    //TODO: to be removed
    [[nodiscard]]
    static auto get_reaction_role_data(size_t message_id, const std::string& reaction_emoji) noexcept -> size_t;
    static auto insert_reaction_role_data(const std::string& role_id, size_t guild_id, const std::string& message_id, const std::string& emoji) noexcept -> void;

    //TODO: to be removed
    [[nodiscard]]
    static auto get_welcome_channel_id(size_t guild_id) noexcept -> size_t;
    static auto insert_welcome_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;   

    //TODO: to be removed
    [[nodiscard]]
    static auto get_goodbye_channel_id(size_t guild_id) noexcept -> size_t;
    static auto insert_goodbye_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;   

    //TODO: to be removed
    [[nodiscard]]
    static auto get_log_channel_id(size_t guild_id) noexcept -> size_t;
    static auto insert_log_channel_id(size_t guild_id, size_t channel_id) noexcept -> void;
};

namespace database {
    // Compile time "for" loop
    template <size_t I = 0, typename... Types>
    constexpr void assignResults(const pqxx::result& result, std::vector<std::variant<Types...>>& args) {
        if constexpr (I < sizeof...(Types)) {
            args[I] = result.at(0, I).get<std::tuple_element_t<I, std::tuple<Types...>>>().value();
            assignResults<I + 1>(result, args);
        }
    }

    template <size_t N, typename ... Types>
    auto execQuery(const std::string& query, Types&...args) noexcept -> void {
        static int times = 0;
        try{
            pqxx::work txn(*Database::getConnection());
            
            // perform the database transaction
            pqxx::result result = txn.exec_params(query, args...);
            txn.commit();
            
            // convert the results into the right datatype and save them
            std::vector<std::variant<int, std::string>> args(N);
            assignResults(result, args);
 
            times=0;
            return;
        } catch(const pqxx::broken_connection& e) {
            ++times;
            if(times > 10){
                times = 0;
                return;
            }
            Database::reconnect();
            execQuery<N>(query, args...);
        } catch (...) {
            //Error case...
        }
    }
}