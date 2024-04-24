#include "BlacklistDTO.hpp"

#include <string>
#include <utility>

BlacklistDTO::BlacklistDTO(std::string username) : username{std::move(username)} {
}

BlacklistDTO::BlacklistDTO(const RowDTOAdapter& row) : username{row.get<std::string>("username")} {
}