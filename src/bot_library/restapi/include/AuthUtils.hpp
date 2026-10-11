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

#include <chrono>
#include <cstddef>
#include <deque>
#include <istream>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Poco/Net/HTTPServerRequest.h"

constexpr std::size_t k_jwt_secret_min_bytes{32};
constexpr std::size_t k_password_min_length{8};
constexpr std::size_t k_password_max_length{128};
constexpr std::size_t k_username_max_length{64};
constexpr std::size_t k_email_max_length{254};
constexpr std::size_t k_auth_body_max_bytes{8 * 1024};

[[nodiscard]] bool constant_time_equal(std::string_view a, std::string_view b) noexcept;

[[nodiscard]] bool jwt_secret_is_strong(std::string_view secret) noexcept;

[[nodiscard]] bool email_address_is_plausible(std::string_view email) noexcept;

[[nodiscard]] bool registration_fields_valid(std::string_view username,
											 std::string_view email,
											 std::string_view password) noexcept;

[[nodiscard]] std::string hashPassword(const std::string& password);

[[nodiscard]] bool verifyPassword(const std::string& password, const std::string& storedHash);

[[nodiscard]] bool jwt_grants_clearance(const std::string& jwt, const std::string& hmac_secret, int min_clearance);

[[nodiscard]] bool declared_body_too_large(const Poco::Net::HTTPServerRequest& request,
										   std::size_t max_bytes) noexcept;

[[nodiscard]] std::optional<std::string> read_limited(std::istream& input, std::size_t max_bytes);

[[nodiscard]] std::string http_client_key(const Poco::Net::HTTPServerRequest& request);

class RequestRateLimiter {
  public:
	RequestRateLimiter(std::size_t max_events, std::chrono::milliseconds window);

	[[nodiscard]] bool allow(std::string_view key);

  private:
	std::size_t max_events_;
	std::chrono::milliseconds window_;
	mutable std::mutex mutex_;
	std::unordered_map<std::string, std::deque<std::chrono::steady_clock::time_point>> hits_;
};

[[nodiscard]] RequestRateLimiter& registration_rate_limiter();

[[nodiscard]] RequestRateLimiter& auth_rate_limiter();

enum class AuthClearance { NONE = 0, PUBLIC = 10, PRIVATE = 100, SECRET = 1000, TOP_SECRET = 10000, BLACK = 100000 };

struct AuthUtil {
	[[nodiscard]] static bool is_authorized(const Poco::Net::HTTPServerRequest& req,
											AuthClearance minClearance = AuthClearance::SECRET);
};
