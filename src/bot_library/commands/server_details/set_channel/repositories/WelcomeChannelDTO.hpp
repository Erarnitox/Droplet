#pragma once

#include <RowDTOAdapter.hpp>
#include <string>

class WelcomeChannelDTO {
  public:
	size_t guild_id;
	size_t channel_id;

	WelcomeChannelDTO() = default;
	WelcomeChannelDTO(size_t guild_id, size_t channel_id);
	WelcomeChannelDTO(const RowDTOAdapter& row);
};