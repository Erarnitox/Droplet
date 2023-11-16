#include "GoodbyeChannelDTO.hpp"

GoodbyeChannelDTO::GoodbyeChannelDTO(size_t guild_id, size_t channel_id) : guild_id{guild_id}, channel_id{channel_id} {
}

GoodbyeChannelDTO::GoodbyeChannelDTO(const RowDTOAdapter& row)
	: guild_id{row.get<size_t>("guild_id")}, channel_id{row.get<size_t>("channel_id")} {
}