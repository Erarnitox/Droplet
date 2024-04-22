#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class ProductDTO {
  public:
	size_t id;
	std::string title;
	std::string description;
	std::string url;
	std::string creator;

	ProductDTO() = default;
	ProductDTO(std::string title, std::string description, std::string url, std::string creator);

	ProductDTO(const RowDTOAdapter& row);
};