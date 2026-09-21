/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "AuthUtils.hpp"

#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <array>
#include <sstream>
#include <stdexcept>

#include "Poco/HexBinaryEncoder.h"
#include "Poco/JWT/Signer.h"
#include "Poco/JWT/Token.h"
#include "Poco/SHA2Engine.h"
#include "Poco/Timestamp.h"
#include "Secrets.hpp"

namespace {

constexpr int k_pbkdf2_iterations{100000};
constexpr std::size_t k_salt_len{16};
constexpr std::size_t k_dk_len{32};

[[nodiscard]] std::string to_hex(const unsigned char* data, std::size_t len) {
	static constexpr char k_hex[]{"0123456789abcdef"};
	std::string out(len * 2, '\0');
	for (std::size_t i{0}; i < len; ++i) {
		out[i * 2] = k_hex[data[i] >> 4];
		out[i * 2 + 1] = k_hex[data[i] & 0x0f];
	}
	return out;
}

[[nodiscard]] bool from_hex(std::string_view hex, unsigned char* out, std::size_t out_len) {
	if (hex.size() != out_len * 2) {
		return false;
	}
	const auto nibble = [](char c) -> int {
		if (c >= '0' && c <= '9') {
			return c - '0';
		}
		if (c >= 'a' && c <= 'f') {
			return c - 'a' + 10;
		}
		if (c >= 'A' && c <= 'F') {
			return c - 'A' + 10;
		}
		return -1;
	};
	for (std::size_t i{0}; i < out_len; ++i) {
		const int hi{nibble(hex[i * 2])};
		const int lo{nibble(hex[i * 2 + 1])};
		if (hi < 0 || lo < 0) {
			return false;
		}
		out[i] = static_cast<unsigned char>((hi << 4) | lo);
	}
	return true;
}

[[nodiscard]] std::string sha256_hex(const std::string& password) {
	Poco::SHA2Engine sha256;
	sha256.update(password);
	const Poco::DigestEngine::Digest& digest = sha256.digest();
	std::stringstream ss;
	Poco::HexBinaryEncoder hexEncoder(ss);
	hexEncoder.write(reinterpret_cast<const char*>(digest.data()), static_cast<long>(digest.size()));
	hexEncoder.close();
	return ss.str();
}

[[nodiscard]] bool verify_pbkdf2(const std::string& password, std::string_view stored) {
	if (!stored.starts_with("v2$")) {
		return false;
	}
	const auto rest{stored.substr(3)};
	const auto sep{rest.find('$')};
	if (sep == std::string_view::npos) {
		return false;
	}
	const auto salt_hex{rest.substr(0, sep)};
	const auto dk_hex{rest.substr(sep + 1)};

	std::array<unsigned char, k_salt_len> salt{};
	std::array<unsigned char, k_dk_len> expected{};
	if (!from_hex(salt_hex, salt.data(), salt.size()) || !from_hex(dk_hex, expected.data(), expected.size())) {
		return false;
	}

	std::array<unsigned char, k_dk_len> actual{};
	if (PKCS5_PBKDF2_HMAC(password.data(),
						  static_cast<int>(password.size()),
						  salt.data(),
						  static_cast<int>(salt.size()),
						  k_pbkdf2_iterations,
						  EVP_sha256(),
						  static_cast<int>(actual.size()),
						  actual.data()) != 1) {
		return false;
	}
	return CRYPTO_memcmp(actual.data(), expected.data(), actual.size()) == 0;
}

}  // namespace

bool constant_time_equal(std::string_view a, std::string_view b) noexcept {
	const std::size_t n{a.size() > b.size() ? a.size() : b.size()};
	unsigned char acc{static_cast<unsigned char>(a.size() != b.size())};
	for (std::size_t i{0}; i < n; ++i) {
		const unsigned char ca = i < a.size() ? static_cast<unsigned char>(a[i]) : 0;
		const unsigned char cb = i < b.size() ? static_cast<unsigned char>(b[i]) : 0;
		acc = static_cast<unsigned char>(acc | (ca ^ cb));
	}
	return acc == 0;
}

bool jwt_secret_is_strong(std::string_view secret) noexcept {
	return secret.size() >= k_jwt_secret_min_bytes;
}

std::string hashPassword(const std::string& password) {
	std::array<unsigned char, k_salt_len> salt{};
	if (RAND_bytes(salt.data(), static_cast<int>(salt.size())) != 1) {
		throw std::runtime_error("Failed to generate password salt");
	}

	std::array<unsigned char, k_dk_len> dk{};
	if (PKCS5_PBKDF2_HMAC(password.data(),
						  static_cast<int>(password.size()),
						  salt.data(),
						  static_cast<int>(salt.size()),
						  k_pbkdf2_iterations,
						  EVP_sha256(),
						  static_cast<int>(dk.size()),
						  dk.data()) != 1) {
		throw std::runtime_error("Failed to derive password hash");
	}

	return std::string("v2$") + to_hex(salt.data(), salt.size()) + "$" + to_hex(dk.data(), dk.size());
}

bool verifyPassword(const std::string& password, const std::string& storedHash) {
	if (storedHash.empty()) {
		return false;
	}
	if (storedHash.starts_with("v2$")) {
		return verify_pbkdf2(password, storedHash);
	}
	return constant_time_equal(sha256_hex(password), storedHash);
}

bool jwt_grants_clearance(const std::string& jwt, const std::string& hmac_secret, int min_clearance) {
	if (!jwt_secret_is_strong(hmac_secret) || jwt.empty()) {
		return false;
	}

	try {
		Poco::JWT::Signer signer(hmac_secret);
		Poco::JWT::Token token;
		if (!signer.tryVerify(jwt, token)) {
			return false;
		}

		const auto expiration{token.getExpiration()};
		if (expiration.epochMicroseconds() == 0 || expiration < Poco::Timestamp()) {
			return false;
		}

		const auto& claims{token.payload()};
		if (!claims.has("prv")) {
			return false;
		}
		return claims.get("prv").convert<int>() >= min_clearance;
	} catch (...) {
		return false;
	}
}

bool AuthUtil::is_authorized(const Poco::Net::HTTPServerRequest& req, AuthClearance minClearance) {
	if (minClearance == AuthClearance::NONE) {
		return true;
	}
	if (not req.has("Authorization")) {
		return false;
	}

	const auto auth = req.get("Authorization");
	const std::string bearer = "Bearer ";
	if (auth.rfind(bearer, 0) != 0) {
		return false;
	}

	const std::string jwt = auth.substr(bearer.size());
	const auto secret = Secrets::getInstance().getSecret("jwt_secret");
	return jwt_grants_clearance(jwt, secret, static_cast<int>(minClearance));
}
