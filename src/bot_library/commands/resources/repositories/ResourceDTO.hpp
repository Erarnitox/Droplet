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

	ResourceDTO() = default;
	ResourceDTO(const std::string& title,
				const std::string& category,
				const std::string& description,
				const std::string& url,
				int difficulty,
				size_t guild_id,
				std::string creator,
				size_t creator_id);

	ResourceDTO(const RowDTOAdapter& row);
};