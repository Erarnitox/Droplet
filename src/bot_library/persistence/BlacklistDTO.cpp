/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#include "BlacklistDTO.hpp"

#include <string>
#include <utility>

//-----------------------------------------------------
//
//-----------------------------------------------------
BlacklistDTO::BlacklistDTO(std::string username) : username{std::move(username)} {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
BlacklistDTO::BlacklistDTO(const RowDTOAdapter& row) : username{row.get<std::string>("username")} {
}