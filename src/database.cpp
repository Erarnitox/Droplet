#include "database.hpp"

#include <pqxx/pqxx>
#include <fmt/core.h>

auto Database::connect(
    const std::string& db_name,
    const std::string& user, 
    const std::string& password,
    const std::string& host,
    const std::string& port
) -> bool{
    try{
        pqxx::connection conn = pqxx::connection(fmt::format(
            "dbname={} user={} password={} hostaddr={} port={}",
             db_name,  user,   password,   host,       port
        ));
        if(!conn.is_open()) return false;


    }catch(...){
        return false;
    }

    return true;
}

auto Database::disconnect() -> void {
    //conn.disconnect();
}