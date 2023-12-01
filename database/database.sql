-- DROP SCHEMA public;

CREATE SCHEMA public AUTHORIZATION postgres;

COMMENT ON SCHEMA public IS 'standard public schema';

-- DROP SEQUENCE public.challenge_roles_id_seq;

CREATE SEQUENCE public.challenge_roles_id_seq
	INCREMENT BY 1
	MINVALUE 1
	MAXVALUE 2147483647
	START 1
	CACHE 1
	NO CYCLE;

-- Permissions

ALTER SEQUENCE public.challenge_roles_id_seq OWNER TO erarnitox;
GRANT ALL ON SEQUENCE public.challenge_roles_id_seq TO erarnitox;
GRANT USAGE, SELECT ON SEQUENCE public.challenge_roles_id_seq TO cyberdrop;

-- DROP SEQUENCE public.members_id_seq;

CREATE SEQUENCE public.members_id_seq
	INCREMENT BY 1
	MINVALUE 1
	MAXVALUE 2147483647
	START 1
	CACHE 1
	NO CYCLE;

-- Permissions

ALTER SEQUENCE public.members_id_seq OWNER TO cyberdrop;
GRANT ALL ON SEQUENCE public.members_id_seq TO cyberdrop;

-- DROP SEQUENCE public.reaction_roles_id_seq;

CREATE SEQUENCE public.reaction_roles_id_seq
	INCREMENT BY 1
	MINVALUE 1
	MAXVALUE 2147483647
	START 1
	CACHE 1
	NO CYCLE;

-- Permissions

ALTER SEQUENCE public.reaction_roles_id_seq OWNER TO erarnitox;
GRANT ALL ON SEQUENCE public.reaction_roles_id_seq TO erarnitox;
GRANT USAGE, SELECT ON SEQUENCE public.reaction_roles_id_seq TO cyberdrop;

-- DROP SEQUENCE public.resources_id_seq;

CREATE SEQUENCE public.resources_id_seq
	INCREMENT BY 1
	MINVALUE 1
	MAXVALUE 2147483647
	START 1
	CACHE 1
	NO CYCLE;

-- Permissions

ALTER SEQUENCE public.resources_id_seq OWNER TO cyberdrop;
GRANT ALL ON SEQUENCE public.resources_id_seq TO cyberdrop;
-- public.challenge_badges definition

-- Drop table

-- DROP TABLE public.challenge_badges;

CREATE TABLE public.challenge_badges (
	badge_emoji varchar NOT NULL,
	"exp" int8 NOT NULL,
	flag varchar NOT NULL,
	message_id int8 NOT NULL,
	guild_id int8 NOT NULL,
	guild_name varchar NOT NULL,
	CONSTRAINT challenge_badges_pkey PRIMARY KEY (message_id)
);

-- Permissions

ALTER TABLE public.challenge_badges OWNER TO cyberdrop;
GRANT ALL ON TABLE public.challenge_badges TO cyberdrop;


-- public.challenge_roles definition

-- Drop table

-- DROP TABLE public.challenge_roles;

CREATE TABLE public.challenge_roles (
	role_id int8 NULL,
	id serial4 NOT NULL,
	guild_id int8 NOT NULL,
	message_id int8 NOT NULL,
	flag varchar NULL,
	CONSTRAINT challenge_roles_pkey PRIMARY KEY (id)
);

-- Permissions

ALTER TABLE public.challenge_roles OWNER TO erarnitox;
GRANT ALL ON TABLE public.challenge_roles TO erarnitox;
GRANT ALL ON TABLE public.challenge_roles TO cyberdrop;


-- public.goodbye_channels definition

-- Drop table

-- DROP TABLE public.goodbye_channels;

CREATE TABLE public.goodbye_channels (
	guild_id numeric NOT NULL,
	channel_id numeric NULL,
	CONSTRAINT goodbye_channels_pkey PRIMARY KEY (guild_id)
);

-- Permissions

ALTER TABLE public.goodbye_channels OWNER TO erarnitox;
GRANT ALL ON TABLE public.goodbye_channels TO erarnitox;
GRANT UPDATE, SELECT, DELETE, INSERT ON TABLE public.goodbye_channels TO cyberdrop;


-- public.log_channels definition

-- Drop table

-- DROP TABLE public.log_channels;

CREATE TABLE public.log_channels (
	guild_id numeric NOT NULL,
	channel_id numeric NULL,
	CONSTRAINT log_channels_pkey PRIMARY KEY (guild_id)
);

-- Permissions

ALTER TABLE public.log_channels OWNER TO erarnitox;
GRANT ALL ON TABLE public.log_channels TO erarnitox;
GRANT UPDATE, SELECT, DELETE, INSERT ON TABLE public.log_channels TO cyberdrop;


-- public.members definition

-- Drop table

-- DROP TABLE public.members;

CREATE TABLE public.members (
	id serial4 NOT NULL,
	discord_id int8 NULL,
	picture_url varchar NULL,
	discord varchar NULL
);

-- Permissions

ALTER TABLE public.members OWNER TO cyberdrop;
GRANT ALL ON TABLE public.members TO cyberdrop;


-- public.reaction_roles definition

-- Drop table

-- DROP TABLE public.reaction_roles;

CREATE TABLE public.reaction_roles (
	id serial4 NOT NULL,
	role_id int8 NULL,
	message_id int8 NULL,
	guild_id int8 NULL,
	emoji varchar NULL
);

-- Permissions

ALTER TABLE public.reaction_roles OWNER TO erarnitox;
GRANT ALL ON TABLE public.reaction_roles TO erarnitox;
GRANT UPDATE, SELECT, DELETE, INSERT ON TABLE public.reaction_roles TO cyberdrop;


-- public.resources definition

-- Drop table

-- DROP TABLE public.resources;

CREATE TABLE public.resources (
	id serial4 NOT NULL,
	title varchar NULL,
	category varchar NULL,
	description varchar NULL,
	url varchar NULL,
	difficulty int8 NULL,
	guild_id int8 NULL,
	creator varchar NULL,
	creator_id int8 NULL
);

-- Permissions

ALTER TABLE public.resources OWNER TO cyberdrop;
GRANT ALL ON TABLE public.resources TO cyberdrop;


-- public.usr definition

-- Drop table

-- DROP TABLE public.usr;

CREATE TABLE public.usr (
	"exp" int8 NOT NULL,
	user_name varchar NULL,
	user_id int8 NOT NULL,
	color varchar NOT NULL,
	is_subscribed int8 NULL,
	CONSTRAINT usr_pkey PRIMARY KEY (user_id)
);

-- Permissions

ALTER TABLE public.usr OWNER TO cyberdrop;
GRANT ALL ON TABLE public.usr TO cyberdrop;


-- public.welcome_channels definition

-- Drop table

-- DROP TABLE public.welcome_channels;

CREATE TABLE public.welcome_channels (
	guild_id numeric NOT NULL,
	channel_id numeric NULL,
	CONSTRAINT welcome_channels_pkey PRIMARY KEY (guild_id)
);

-- Permissions

ALTER TABLE public.welcome_channels OWNER TO erarnitox;
GRANT ALL ON TABLE public.welcome_channels TO erarnitox;
GRANT UPDATE, SELECT, DELETE, INSERT ON TABLE public.welcome_channels TO cyberdrop;


-- public.has_badge definition

-- Drop table

-- DROP TABLE public.has_badge;

CREATE TABLE public.has_badge (
	message_id int8 NOT NULL,
	user_id int8 NOT NULL,
	CONSTRAINT has_badge_pkey PRIMARY KEY (message_id, user_id),
	CONSTRAINT has_badge_message_id_fkey FOREIGN KEY (message_id) REFERENCES public.challenge_badges(message_id),
	CONSTRAINT has_badge_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.usr(user_id)
);

-- Permissions

ALTER TABLE public.has_badge OWNER TO cyberdrop;
GRANT ALL ON TABLE public.has_badge TO cyberdrop;




-- Permissions

GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO public;
GRANT ALL ON SCHEMA public TO cyberdrop;