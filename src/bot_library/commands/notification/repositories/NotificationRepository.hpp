/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description:
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/
 */

#pragma once

#include "IRepository.hpp"
#include "NotificationDTO.hpp"

class DatabaseExecutor;

//-----------------------------------------------------
//
//-----------------------------------------------------
class NotificationRepository : public IRepository<NotificationDTO> {
  public:
	NotificationRepository();
	explicit NotificationRepository(DatabaseExecutor& executor);

	[[nodiscard]] bool create(const NotificationDTO& object) noexcept override;

	[[nodiscard]] bool remove(size_t id) noexcept override;

	[[nodiscard]] NotificationDTO get(size_t id) const noexcept override;

	[[nodiscard]] bool update(const NotificationDTO& object) noexcept override;

	[[nodiscard]] std::vector<NotificationDTO> getAll() const noexcept;

  private:
	DatabaseExecutor& executor_;
};