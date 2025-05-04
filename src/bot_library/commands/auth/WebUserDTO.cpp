#include "WebUserDTO.hpp"

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

WebUserDTO::WebUserDTO(const RowDTOAdapter& row)
	: id{row.get<size_t>("id")},
	  username{row.get<std::string>("username")},
	  password{row.get<std::string>("password")},
	  clearance{row.get<size_t>("clearance")},
	  email{row.get<std::string>("email")},
	  confirm_code{row.get<std::string>("confirm_code")},
	  is_verified{row.get<bool>("is_verified")} {
}