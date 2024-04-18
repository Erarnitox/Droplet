#include "TextDTO.hpp"

#include <utility>

TextDTO::TextDTO(std::string title, std::string description, std::string url, std::string creator)
	: id{0},
	  title{std::move(title)},
	  description{std::move(description)},
	  url{std::move(url)},
	  creator{std::move(creator)} {
}

TextDTO::TextDTO(const RowDTOAdapter& row)
	: id{row.get<size_t>("id")},
	  title{row.get<std::string>("title")},
	  description{row.get<std::string>("description")},
	  url{row.get<std::string>("url")},
	  creator{row.get<std::string>("creator")} {
}