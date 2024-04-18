#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class TextDTO {
  public:
	size_t id;
	std::string title;
	std::string description;
	std::string url;
	std::string creator;

	TextDTO() = default;
	TextDTO(std::string title, std::string description, std::string url, std::string creator);

	TextDTO(const RowDTOAdapter& row);
};