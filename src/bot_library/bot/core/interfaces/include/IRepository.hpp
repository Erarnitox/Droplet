/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <cstddef>

template <typename DTO>
class IRepositoryReader {
  public:
	virtual ~IRepositoryReader() = default;

	[[nodiscard]] virtual DTO get(size_t id) const noexcept = 0;
};

template <typename DTO>
class IRepositoryWriter {
  public:
	virtual ~IRepositoryWriter() = default;

	[[nodiscard]] virtual bool create(const DTO& object) noexcept = 0;

	[[nodiscard]] virtual bool remove(size_t index) noexcept = 0;

	[[nodiscard]] virtual bool update(const DTO& object) noexcept = 0;
};

/**
 * Full CRUD repository surface; aggregates reader + writer (ISP-friendly split above).
 */
template <typename DTO>
class IRepository : public IRepositoryReader<DTO>, public IRepositoryWriter<DTO> {};
