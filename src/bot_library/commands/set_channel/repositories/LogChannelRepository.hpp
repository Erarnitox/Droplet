#pragma once

#include <IRepository.hpp>

#include "LogChannelDTO.hpp"

class LogChannelRepository : public IRepository<LogChannelDTO> {
  public:
	[[nodiscard]] bool create(const LogChannelDTO& object) override;

	[[nodiscard]] bool remove(size_t guild_id) override;

	[[nodiscard]] LogChannelDTO get(size_t guild_id) override;

	[[nodiscard]] bool update(const LogChannelDTO& object) override;
};