#pragma once

#include "IRepository.hpp"
#include "MemberDTO.hpp"

class MemberRepository : public IRepository<MemberDTO> {
  public:
	[[nodiscard]] bool create(const MemberDTO& object) override;

	[[nodiscard]] bool remove(size_t id) override;

	[[nodiscard]] MemberDTO get(size_t id) override;

	[[nodiscard]] bool update(const MemberDTO& object) override;
};