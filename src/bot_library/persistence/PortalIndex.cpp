#include "PortalIndex.hpp"

#include "PortalDTO.hpp"

void PortalIndex::rebuild(const std::vector<PortalDTO>& portals) {
	std::lock_guard<std::mutex> lock(mutex_);
	channel_ids_.clear();
	for (const auto& portal : portals) {
		channel_ids_.insert(static_cast<std::uint64_t>(portal.channel_id));
	}
}

void PortalIndex::add(std::uint64_t channel_id) {
	std::lock_guard<std::mutex> lock(mutex_);
	channel_ids_.insert(channel_id);
}

void PortalIndex::remove(std::uint64_t channel_id) {
	std::lock_guard<std::mutex> lock(mutex_);
	channel_ids_.erase(channel_id);
}

bool PortalIndex::contains(std::uint64_t channel_id) const {
	std::lock_guard<std::mutex> lock(mutex_);
	return channel_ids_.contains(channel_id);
}

void BlacklistIndex::rebuild(const std::vector<std::string>& usernames) {
	std::lock_guard<std::mutex> lock(mutex_);
	usernames_.clear();
	for (const auto& name : usernames) {
		usernames_.insert(name);
	}
}

void BlacklistIndex::add(std::string username) {
	std::lock_guard<std::mutex> lock(mutex_);
	usernames_.insert(std::move(username));
}

void BlacklistIndex::remove(const std::string& username) {
	std::lock_guard<std::mutex> lock(mutex_);
	usernames_.erase(username);
}

bool BlacklistIndex::contains(const std::string& username) const {
	std::lock_guard<std::mutex> lock(mutex_);
	return usernames_.contains(username);
}
