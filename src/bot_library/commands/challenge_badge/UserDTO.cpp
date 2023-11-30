#include "UserDTO.hpp"

UserDTO::UserDTO(size_t user_id,
				 const std::string& user_name,
				 const std::string& color,
				 size_t exp,
				 size_t is_subscribed)
	: user_id{user_id}, user_name{user_name}, color{color}, exp{exp}, is_subscribed{is_subscribed} {
}

UserDTO::UserDTO(const RowDTOAdapter& row)
	: user_id{row.get<size_t>("user_id")},
	  user_name{row.get<std::string>("user_name")},
	  color{row.get<std::string>("color")},
	  exp{row.get<size_t>("exp")},
	  is_subscribed{row.get<size_t>("is_subscribed")} {
}