#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class LogChannelDTO {
  public:
	size_t guild_id;
	size_t channel_id;

	LogChannelDTO() = default;
	LogChannelDTO(size_t guild_id, size_t channel_id);
	LogChannelDTO(const RowDTOAdapter& row);
};