#include "UserDTO.hpp"

UserDTO::UserDTO(size_t user_id, const std::string& color, size_t exp, bool is_subscribed)
	: user_id{user_id}, color{color}, exp{exp}, is_subscribed{is_subscribed} {
}

UserDTO::UserDTO(const RowDTOAdapter& row)
	: user_id{row.get<size_t>("user_id")},
	  color{row.get<std::string>("color")},
	  exp{row.get<size_t>("exp")},
	  is_subscribed{row.get<bool>("is_subscribed")} {
}