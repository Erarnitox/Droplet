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

//-----------------------------------------------------
//
//-----------------------------------------------------
class UserDTO {
  public:
	size_t user_id;
	std::string user_name;
	std::string color;
	size_t exp;
	size_t is_subscribed;

	UserDTO() = default;
	UserDTO(size_t user_id,
			const std::string& user_name,
			const std::string& color = "00FF00",
			size_t exp = 0,
			size_t is_subscribed = 0);

	explicit UserDTO(const RowDTOAdapter& row);
};