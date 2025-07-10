#pragma once

#include <cstddef>
template <typename DTO>
class IRepository {
  public:
	[[nodiscard]] virtual bool create(const DTO& object) noexcept = 0;

	[[nodiscard]] virtual bool remove(size_t index) noexcept = 0;

	[[nodiscard]] virtual DTO get(size_t index) noexcept = 0;

	[[nodiscard]] virtual bool update(const DTO& object) noexcept = 0;
};