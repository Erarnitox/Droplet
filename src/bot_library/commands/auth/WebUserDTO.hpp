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

class WebUserDTO {
  public:
	size_t id;
	std::string username;
	std::string password;
	size_t clearance;
	std::string email;
	std::string confirm_code;
	bool is_verified;

	WebUserDTO() = default;
	WebUserDTO(size_t id,
			   const std::string& username,
			   const std::string& password,
			   size_t clearance,
			   const std::string& email,
			   const std::string& confirm_code = "",
			   bool is_verified = false);
	explicit WebUserDTO(const RowDTOAdapter& row);
};