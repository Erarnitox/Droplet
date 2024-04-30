#include "ProductDTO.hpp"

#include <utility>

ProductDTO::ProductDTO(size_t product_id, std::string picture_url, std::string button_text, 
			   		   size_t price_in_cents, std::string name, std::string button_url, 
			   		   std::string buy_url, Type type)
	: id{0},
	  product_id{ product_id },
	  picture_url{ std::move(picture_url) },
	  button_text{ std::move(button_text) },
	  price_in_cents{ price_in_cents },
	  name{ std::move(name) },
	  button_url{ std::move(button_url) },
	  buy_url{ std::move(buy_url) },
	  type{ type }
{
}

ProductDTO::ProductDTO(const RowDTOAdapter& row)
	: id{row.get<size_t>("id")},
	  product_id{ row.get<size_t>("product_id") },
	  picture_url{ row.get<std::string>("picture_url") },
	  button_text{ row.get<std::string>("button_text") },
	  price_in_cents{ row.get<size_t>("price_in_cents") },
	  name{ row.get<std::string>("name") },
	  button_url{ row.get<std::string>("button_url") },
	  buy_url{ row.get<std::string>("buy_url") },
	  type{ static_cast<Type>(row.get<size_t>("type")) } {
}