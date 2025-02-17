#include "NotificationDTO.hpp"

#include <string>

NotificationDTO::NotificationDTO(size_t guild_id,
								 size_t channel_id,
								 const std::string& type,
								 const std::string& data,
								 const std::string& message,
								 size_t timestep)
	: guild_id{guild_id}, channel_id{channel_id}, timestep{timestep}, type{type}, data{data}, message{message} {
}

NotificationDTO::NotificationDTO(const RowDTOAdapter& row)
	: guild_id{row.get<size_t>("guild_id")},
	  channel_id{row.get<size_t>("channel_id")},
	  timestep{row.get<size_t>("timestep")},
	  type{row.get<std::string>("type")},
	  data{row.get<std::string>("type")},
	  message{row.get<std::string>("message")} {
}