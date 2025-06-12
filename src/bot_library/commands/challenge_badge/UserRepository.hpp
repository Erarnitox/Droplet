#pragma once

#include <IRepository.hpp>

#include "UserDTO.hpp"

class UserRepository : public IRepository<UserDTO> {
  public:
	[[nodiscard]] bool create(const UserDTO& object) override;

	[[nodiscard]] bool remove(size_t user_id) override;

	[[nodiscard]] UserDTO get(size_t user_id) override;

	[[nodiscard]] bool update(const UserDTO& object) override;

	[[nodiscard]] std::vector<UserDTO> getTopTen();
};