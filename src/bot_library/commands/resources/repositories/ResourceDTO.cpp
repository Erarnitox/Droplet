#include "ResourceDTO.hpp"

ResourceDTO::ResourceDTO(const std::string& title,
						 const std::string& category,
						 const std::string& description,
						 const std::string& url,
						 int difficulty,
						 size_t guild_id,
						 std::string creator,
						 size_t creator_id)
	: id{0},
	  title{title},
	  category{category},
	  description{description},
	  url{url},
	  difficulty{difficulty},
	  guild_id{guild_id},
	  creator{creator},
	  creator_id{creator_id} {
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
	  creator_id{row.get<size_t>("creator_id")} {
}