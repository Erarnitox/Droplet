/**
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: Initializes the Database Tables
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/init_databse
 **/

#include "InitDatabase.hpp"

/**
 * @brief Initializes the Database Tables once the bot is ready
 *
 * @param event the ready event
 * @return doesn't return anything
 */
void InitDatabase::on_ready(const dpp::ready_t& event) {
	auto result = database::execQuery(
		R"ERARNITOX(
CREATE SEQUENCE IF NOT EXISTS public.challenge_roles_id_seq
	INCREMENT BY 1
	MINVALUE 1
	MAXVALUE 2147483647
	START 1
	CACHE 1
	NO CYCLE;

CREATE SEQUENCE IF NOT EXISTS public.reaction_roles_id_seq
	INCREMENT BY 1
	MINVALUE 1
	MAXVALUE 2147483647
	START 1
	CACHE 1
	NO CYCLE;

CREATE TABLE IF NOT EXISTS public.challenge_roles (
	role_id int8 NULL,
	id serial4 NOT NULL,
	guild_id int8 NOT NULL,
	message_id int8 NOT NULL,
	flag varchar NULL,
	CONSTRAINT challenge_roles_pkey PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS public.goodbye_channels (
	guild_id numeric NOT NULL,
	channel_id numeric NULL,
	CONSTRAINT goodbye_channels_pkey PRIMARY KEY (guild_id)
);

CREATE TABLE IF NOT EXISTS public.log_channels (
	guild_id numeric NOT NULL,
	channel_id numeric NULL,
	CONSTRAINT log_channels_pkey PRIMARY KEY (guild_id)
);

CREATE TABLE IF NOT EXISTS public.reaction_roles (
	id serial4 NOT NULL,
	role_id int8 NULL,
	message_id int8 NULL,
	guild_id int8 NULL,
	emoji varchar NULL
);

CREATE TABLE IF NOT EXISTS public.welcome_channels (
	guild_id numeric NOT NULL,
	channel_id numeric NULL,
	CONSTRAINT welcome_channels_pkey PRIMARY KEY (guild_id)
);
)ERARNITOX");

	if (result) {
		std::puts("Database got initialized!");
	} else {
		std::puts("Database initialization failed!");
	}

	return;
}