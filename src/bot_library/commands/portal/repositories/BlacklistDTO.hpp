#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

/*
CREATE TABLE public.blacklist (
	id serial4 NOT NULL,
	username varchar null
);
*/
class BlacklistDTO {
  public:
	std::string username;

	BlacklistDTO() = default;
	BlacklistDTO(std::string username);

	BlacklistDTO(const RowDTOAdapter& row);
};