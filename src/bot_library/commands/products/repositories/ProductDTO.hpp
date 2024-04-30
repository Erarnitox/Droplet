#pragma once

#include <RowDTOAdapter.hpp>
#include <cstddef>
#include <string>

/*
CREATE TABLE public.products (
	id serial4 NOT NULL,
	product_id int8 null,
	picture_url varchar null,
	button_text varchar null,
	price_in_cents int8 null,
	name varchar null,
	button_url varchar null,
	buy_url varchar null,
	type int8 null
);
*/
class ProductDTO {
  public:
	enum Type { DOWNLOAD = 0, SUBSCRIPTION = 100 };

	size_t id;
	size_t product_id;
	std::string picture_url;
	std::string button_text;
	size_t price_in_cents;
	std::string name;
	std::string button_url;
	std::string buy_url;
	Type type;

	ProductDTO() = default;
	ProductDTO(size_t product_id,
			   std::string picture_url,
			   std::string button_text,
			   size_t price_in_cents,
			   std::string name,
			   std::string button_url,
			   std::string buy_url,
			   Type type);

	ProductDTO(const RowDTOAdapter& row);
};