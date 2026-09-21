#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

/*
CREATE TABLE public.notifications
(
  id serial4 NOT NULL,
  guild_id int8 null,
  channel_id int8 null,
  type varchar,
  data varchar,
  message varchar,
  timestep int8 null
);

*/
class NotificationDTO {
  public:
	size_t guild_id;
	size_t channel_id;
	size_t timestep;
	std::string type;
	std::string data;
	std::string message;

	NotificationDTO() = default;
	NotificationDTO(size_t guild_id,
					size_t channel_id,
					const std::string& type,
					const std::string& data,
					const std::string& message,
					size_t timestep = 3600);

	NotificationDTO(const RowDTOAdapter& row);
};