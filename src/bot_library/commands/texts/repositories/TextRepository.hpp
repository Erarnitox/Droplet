#pragma once

#include <IRepository.hpp>

#include "TextDTO.hpp"

class TextRepository : public IRepository<TextDTO> {
  public:
	[[nodiscard]] auto create(const TextDTO& object) -> bool override;

	[[nodiscard]] auto remove(size_t id) -> bool override;

	[[nodiscard]] auto get(size_t id) -> TextDTO override;

	[[nodiscard]] auto update(const TextDTO& object) -> bool override;
};