#include "MemberDTO.hpp"

#include <string>

MemberDTO::MemberDTO(size_t discord_id, const std::string& picture_url, const std::string& username)
	: discord_id{discord_id}, picture_url{picture_url}, username{username} {
}

MemberDTO::MemberDTO(const RowDTOAdapter& row)
	: discord_id{row.get<size_t>("discord_id")},
	  picture_url{row.get<std::string>("picture_url")},
	  username{row.get<std::string>("username")} {
}