#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class UserDTO {
  public:
	size_t user_id;
	std::string color;
	size_t exp;
	bool is_subscribed;

	UserDTO() = default;
	UserDTO(size_t user_id, const std::string& color = "00FF00", size_t exp = 0, bool is_subscribed = false);
	UserDTO(const RowDTOAdapter& row);
};