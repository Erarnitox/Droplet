#pragma once

#include "IRepository.hpp"
#include "PortalDTO.hpp"

class PortalRepository : public IRepository<PortalDTO> {
  public:
	[[nodiscard]] auto create(const PortalDTO& object) -> bool override;

	[[nodiscard]] auto remove(size_t id) -> bool override;

	[[nodiscard]] auto get(size_t id) -> PortalDTO override;

	[[nodiscard]] auto update(const PortalDTO& object) -> bool override;

	[[nodiscard]] auto getAll() -> std::vector<PortalDTO>;
};