#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class GoodbyeChannelDTO {
  public:
	size_t guild_id;
	size_t channel_id;

	GoodbyeChannelDTO() = default;
	GoodbyeChannelDTO(size_t guild_id, size_t channel_id);
	GoodbyeChannelDTO(const RowDTOAdapter& row);
};