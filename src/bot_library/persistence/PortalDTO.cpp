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

#include "PortalDTO.hpp"

#include <string>

//-----------------------------------------------------
//
//-----------------------------------------------------
PortalDTO::PortalDTO(size_t guild_id, size_t channel_id) : guild_id{guild_id}, channel_id{channel_id} {
}

//-----------------------------------------------------
//
//-----------------------------------------------------
PortalDTO::PortalDTO(const RowDTOAdapter& row)
	: guild_id{row.get<size_t>("guild_id")}, channel_id{row.get<size_t>("channel_id")} {
}