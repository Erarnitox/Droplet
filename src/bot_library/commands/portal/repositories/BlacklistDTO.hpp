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
CREATE TABLE public.blacklist (
	id serial4 NOT NULL,
	username varchar null
);
*/

//-----------------------------------------------------
//
//-----------------------------------------------------
class BlacklistDTO {
  public:
	std::string username;

	BlacklistDTO() = default;
	explicit BlacklistDTO(std::string username);

	explicit BlacklistDTO(const RowDTOAdapter& row);
};