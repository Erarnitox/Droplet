#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

class PortalDTO;

class PortalIndex {
  public:
	void rebuild(const std::vector<PortalDTO>& portals);
	void add(std::uint64_t channel_id);
	void remove(std::uint64_t channel_id);
	[[nodiscard]] bool contains(std::uint64_t channel_id) const;

  private:
	mutable std::mutex mutex_;
	std::unordered_set<std::uint64_t> channel_ids_;
};

class BlacklistIndex {
  public:
	void rebuild(const std::vector<std::string>& usernames);
	void add(std::string username);
	void remove(const std::string& username);
	[[nodiscard]] bool contains(const std::string& username) const;

  private:
	mutable std::mutex mutex_;
	std::unordered_set<std::string> usernames_;
};
