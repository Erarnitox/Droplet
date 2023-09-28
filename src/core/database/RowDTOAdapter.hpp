#pragma once

#include <pqxx/pqxx>

// Adapter class to convert pqxx::row to GenericDTO
class RowDTOAdapter {
    private:
    const pqxx::row& row_;

    public:
    RowDTOAdapter(const pqxx::row& row) : row_(row) {}

    template<typename T>
    auto get(const std::string& fieldName) -> T {
        return this->row_[fieldName].as<T>();
    }
};