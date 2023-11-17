#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

/*
CREATE TABLE public.members (
	id serial4 NOT NULL,
	discord_id int8 null,
	picture_url varchar null,
	discord varchar null
);
*/
class MemberDTO {
  public:
	size_t discord_id;
	std::string picture_url;
	std::string username;

	MemberDTO() = default;
	MemberDTO(size_t discord_id, const std::string& picture_url, const std::string& username);

	MemberDTO(const RowDTOAdapter& row);
};