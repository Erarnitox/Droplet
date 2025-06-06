#pragma once

#include <IRepository.hpp>
#include <cstddef>

#include "WebUserDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
class WebUserRepository : public IRepository<WebUserDTO> {
  public:
	[[nodiscard]] bool create(const WebUserDTO& object) override;

	[[nodiscard]] bool remove(size_t user_id) override;

	[[nodiscard]] WebUserDTO get(size_t user_id) override;

	[[nodiscard]] WebUserDTO get(const std::string& username);

	[[nodiscard]] std::vector<WebUserDTO> getAll();

	[[nodiscard]] bool verify(const std::string& token, size_t clearance);

	[[nodiscard]] bool exists(const std::string& username);

	[[nodiscard]] bool update(const WebUserDTO& object) override;
};