#include "ProductRepository.hpp"

#include <Database.hpp>
#include <cstddef>
#include <vector>

auto ProductRepository::create(const ProductDTO& object) -> bool {
	static std::string sql_string{
		"INSERT INTO products"
		"(product_id, picture_url, button_text, price_in_cents, name, button_url, buy_url, type) VALUES "
		"($1::int8,   $2::varchar, $3::varchar, $4::int8, $5::varchar, $6::varchar, $7::varchar, $8::int8)"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string,
							   object.product_id,
							   object.picture_url,
							   object.button_text,
							   object.price_in_cents,
							   object.name,
							   object.button_url,
							   object.buy_url,
							   static_cast<size_t>(object.type));
}

auto ProductRepository::remove(size_t id) -> bool {
	static std::string sql_string{"DELETE FROM products WHERE id = $1::int8"};

	if (!Database::hasConnection()) {
		return false;
	}

	return database::execQuery(sql_string, id);
}

auto ProductRepository::update(const ProductDTO& object) -> bool {
	(void)object;
	/*
	static std::string sql_string{"UPDATE welcome_channels SET channel_id = $2::Int8 WHERE guild_id = $1"};

	if (!Database::hasConnection()) {
		return false;
	}

	if (!object.guild_id) {
		return false;
	}

	return database::execQuery(sql_string, object.guild_id, object.channel_id);
	*/
	return false;
}

auto ProductRepository::get(size_t id) -> ProductDTO {
	(void)id;
	/*
	static std::string sql_string{"SELECT channel_id FROM welcome_channels WHERE guild_id=$1::int8"};

	auto result{database::execSelect(sql_string, guild_id)};

	WelcomeChannelDTO dto;
	dto.guild_id = guild_id;
	dto.channel_id = result.get<decltype(dto.channel_id)>("channel_id");

	return dto;
	*/
	return {};
}