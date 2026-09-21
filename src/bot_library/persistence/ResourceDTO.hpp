#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class ResourceDTO {
  public:
	size_t id;
	std::string title;
	std::string category;
	std::string description;
	std::string url;
	int difficulty;
	size_t guild_id;
	std::string creator;
	size_t creator_id;
	std::string tags;

	ResourceDTO() = default;
	ResourceDTO(std::string title,
				std::string category,
				std::string description,
				std::string url,
				int difficulty,
				size_t guild_id,
				std::string creator,
				size_t creator_id,
				std::string tags);

	ResourceDTO(const RowDTOAdapter& row);
};