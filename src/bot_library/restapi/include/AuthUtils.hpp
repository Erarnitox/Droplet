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

#include <cstddef>
#include <string>
#include <string_view>

#include "Poco/Net/HTTPServerRequest.h"

constexpr std::size_t k_jwt_secret_min_bytes{32};
constexpr std::size_t k_password_min_length{8};

[[nodiscard]] bool constant_time_equal(std::string_view a, std::string_view b) noexcept;

[[nodiscard]] bool jwt_secret_is_strong(std::string_view secret) noexcept;

[[nodiscard]] std::string hashPassword(const std::string& password);

[[nodiscard]] bool verifyPassword(const std::string& password, const std::string& storedHash);

[[nodiscard]] bool jwt_grants_clearance(const std::string& jwt, const std::string& hmac_secret, int min_clearance);

enum class AuthClearance { NONE = 0, PUBLIC = 10, PRIVATE = 100, SECRET = 1000, TOP_SECRET = 10000, BLACK = 100000 };

struct AuthUtil {
	[[nodiscard]] static bool is_authorized(const Poco::Net::HTTPServerRequest& req,
											AuthClearance minClearance = AuthClearance::SECRET);
};
