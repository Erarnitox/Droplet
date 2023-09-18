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
GRANT SELECT, USAGE ON SEQUENCE public.challenge_roles_id_seq TO cyberdrop;

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
GRANT SELECT, USAGE ON SEQUENCE public.reaction_roles_id_seq TO cyberdrop;
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
GRANT SELECT, DELETE, UPDATE, INSERT ON TABLE public.goodbye_channels TO cyberdrop;


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
GRANT SELECT, DELETE, UPDATE, INSERT ON TABLE public.log_channels TO cyberdrop;


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
GRANT SELECT, DELETE, UPDATE, INSERT ON TABLE public.reaction_roles TO cyberdrop;


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
GRANT SELECT, DELETE, UPDATE, INSERT ON TABLE public.welcome_channels TO cyberdrop;




-- Permissions

GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO public;
GRANT ALL ON SCHEMA public TO cyberdrop;