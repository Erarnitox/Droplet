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

#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

/*
CREATE TABLE public.portals (
	id serial4 NOT NULL,
	guild_id int8 null,
	channel_id int8 null
);
*/

//-----------------------------------------------------
//
//-----------------------------------------------------
class PortalDTO {
  public:
	size_t guild_id;
	size_t channel_id;

	PortalDTO() = default;
	PortalDTO(size_t guild_id, size_t channel_id);

	explicit PortalDTO(const RowDTOAdapter& row);
};