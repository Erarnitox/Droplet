#pragma once

#include <cstddef>
template <typename DTO>
class IRepository {
  public:
	[[nodiscard]] virtual bool create(const DTO &object) = 0;

	[[nodiscard]] virtual bool remove(size_t index) = 0;

	[[nodiscard]] virtual DTO get(size_t index) = 0;

	[[nodiscard]] virtual bool update(const DTO &object) = 0;
};