#pragma once

#include "BlacklistDTO.hpp"
#include "IRepository.hpp"

class BlacklistRepository : public IRepository<BlacklistDTO> {
  public:
	[[nodiscard]] auto create(const BlacklistDTO& object) -> bool override;

	[[nodiscard]] auto remove(size_t id) -> bool override;

	[[nodiscard]] auto get(size_t id) -> BlacklistDTO override;

	[[nodiscard]] auto update(const BlacklistDTO& object) -> bool override;

	[[nodiscard]] auto getAll() -> std::vector<BlacklistDTO>;
};