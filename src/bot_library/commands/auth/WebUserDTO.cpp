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

#include "WebUserDTO.hpp"

/**
 * @brief Constructor for a WebUserDTO Object
 *
 * @param id the user id
 * @param username the users username
 * @param password the users password
 * @param clearance the users clearance level (used for access control)
 * @param email the users email address
 * @param confirm_code the confirmation code that was sent to the user to validate the email address
 * @param is_verified did the user already verify his email?
 */
WebUserDTO::WebUserDTO(size_t id,
					   const std::string& username,
					   const std::string& password,
					   size_t clearance,
					   const std::string& email,
					   const std::string& confirm_code,
					   bool is_verified)
	: id{id},
	  username{username},
	  password{password},
	  clearance{clearance},
	  email{email},
	  confirm_code{confirm_code},
	  is_verified{is_verified} {
}

/**
 * @brief Constructor to construct a WebUserDTO Object from a Database Row
 *
 * @param row The User-Database Row the WebUserDTO Object should pull its data from
 */
WebUserDTO::WebUserDTO(const RowDTOAdapter& row)
	: id{row.get<size_t>("id")},
	  username{row.get<std::string>("username")},
	  password{row.get<std::string>("password")},
	  clearance{row.get<size_t>("clearance")},
	  email{row.get<std::string>("email")},
	  confirm_code{row.get<std::string>("confirm_code")},
	  is_verified{row.get<bool>("is_verified")} {
}