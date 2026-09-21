#include "ResourceDTO.hpp"

#include <utility>

ResourceDTO::ResourceDTO(std::string title,
						 std::string category,
						 std::string description,
						 std::string url,
						 int difficulty,
						 size_t guild_id,
						 std::string creator,
						 size_t creator_id,
						 std::string tags)
	: id{0},
	  title{std::move(title)},
	  category{std::move(category)},
	  description{std::move(description)},
	  url{std::move(url)},
	  difficulty{difficulty},
	  guild_id{guild_id},
	  creator{std::move(creator)},
	  creator_id{creator_id},
	  tags{std::move(tags)} {
}

ResourceDTO::ResourceDTO(const RowDTOAdapter& row)
	: id{row.get<size_t>("id")},
	  title{row.get<std::string>("title")},
	  category{row.get<std::string>("category")},
	  description{row.get<std::string>("description")},
	  url{row.get<std::string>("url")},
	  difficulty{row.get<int>("difficulty")},
	  guild_id{row.get<size_t>("guild_id")},
	  creator{row.get<std::string>("creator")},
	  creator_id{row.get<size_t>("creator_id")},
	  tags{row.get<std::string>("tags")} {
}