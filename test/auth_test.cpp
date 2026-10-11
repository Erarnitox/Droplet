#include <catch2/catch_test_macros.hpp>

#include <AuthUtils.hpp>
#include <JWTUtils.hpp>
#include <Poco/HexBinaryEncoder.h>
#include <Poco/JSON/Object.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JWT/Token.h>
#include <Poco/SHA2Engine.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
#include <chrono>
#include <sstream>
#include <stdexcept>

namespace {

constexpr const char* k_secret{"0123456789ABCDEF0123456789ABCDEF"};

}  // namespace

TEST_CASE("registration fields reject oversized passwords and unsafe emails", "[auth]") {
	REQUIRE(email_address_is_plausible("person@example.com"));
	REQUIRE_FALSE(email_address_is_plausible("person@example.com>, <other@evil.com"));
	REQUIRE_FALSE(email_address_is_plausible("a@b"));
	REQUIRE_FALSE(email_address_is_plausible("has space@example.com"));
	REQUIRE(registration_fields_valid("alice", "alice@example.com", "long-enough"));
	REQUIRE_FALSE(registration_fields_valid("alice", "alice@example.com", "short"));
	REQUIRE_FALSE(registration_fields_valid("alice", "alice@example.com", std::string(k_password_max_length + 1, 'x')));
	REQUIRE_THROWS_AS(hashPassword(std::string(k_password_max_length + 1, 'x')), std::invalid_argument);
}

TEST_CASE("request rate limiter blocks one client without blocking another", "[auth]") {
	RequestRateLimiter limiter{2, std::chrono::seconds{60}};
	REQUIRE(limiter.allow("10.0.0.1"));
	REQUIRE(limiter.allow("10.0.0.1"));
	REQUIRE_FALSE(limiter.allow("10.0.0.1"));
	REQUIRE(limiter.allow("10.0.0.2"));
}

TEST_CASE("read_limited stops at the configured maximum", "[auth]") {
	std::istringstream input{"0123456789"};
	const auto capped{read_limited(input, 4)};
	REQUIRE_FALSE(capped.has_value());

	std::istringstream exact{"abcd"};
	const auto accepted{read_limited(exact, 4)};
	REQUIRE(accepted.has_value());
	REQUIRE(*accepted == "abcd");
}

TEST_CASE("password hashing uses salted pbkdf2 and rejects mismatches", "[auth]") {
	const auto stored{hashPassword("correct horse battery")};
	REQUIRE(stored.starts_with("v2$"));
	REQUIRE(verifyPassword("correct horse battery", stored));
	REQUIRE_FALSE(verifyPassword("wrong password", stored));
	REQUIRE_FALSE(verifyPassword("correct horse battery", ""));
	REQUIRE_FALSE(verifyPassword("anything", "not-a-hash"));
}

TEST_CASE("legacy sha256 password hashes still verify", "[auth]") {
	Poco::SHA2Engine sha256;
	sha256.update(std::string{"password"});
	const auto digest = sha256.digest();
	std::stringstream ss;
	Poco::HexBinaryEncoder hex(ss);
	hex.write(reinterpret_cast<const char*>(digest.data()), static_cast<long>(digest.size()));
	hex.close();
	REQUIRE(verifyPassword("password", ss.str()));
	REQUIRE_FALSE(verifyPassword("Password", ss.str()));
}

TEST_CASE("jwt_grants_clearance rejects forged empty and expired tokens", "[auth]") {
	REQUIRE_FALSE(jwt_secret_is_strong(""));
	REQUIRE_FALSE(jwt_secret_is_strong("short"));
	REQUIRE(jwt_secret_is_strong(k_secret));

	Poco::JSON::Object claims;
	claims.set("sub", "alice");
	claims.set("prv", static_cast<int>(AuthClearance::PRIVATE));
	const auto jwt{JWTUtils::generateToken(claims, k_secret, "erarnitox.de")};

	REQUIRE(jwt_grants_clearance(jwt, k_secret, static_cast<int>(AuthClearance::PUBLIC)));
	REQUIRE_FALSE(jwt_grants_clearance(jwt, k_secret, static_cast<int>(AuthClearance::SECRET)));
	REQUIRE_FALSE(jwt_grants_clearance(jwt, "0123456789ABCDEF0123456789ABCDE!", static_cast<int>(AuthClearance::PUBLIC)));
	REQUIRE_FALSE(jwt_grants_clearance(jwt, "", static_cast<int>(AuthClearance::PUBLIC)));

	Poco::JWT::Token expired;
	expired.setType("JWT");
	expired.setAlgorithm("HS256");
	expired.setExpiration(Poco::Timestamp() - Poco::Timespan(60, 0));
	expired.payload().set("prv", static_cast<int>(AuthClearance::BLACK));
	const Poco::JWT::Signer signer(k_secret);
	const auto expired_jwt{signer.sign(expired, Poco::JWT::Signer::ALGO_HS256)};
	REQUIRE_FALSE(jwt_grants_clearance(expired_jwt, k_secret, static_cast<int>(AuthClearance::PUBLIC)));

	Poco::JWT::Token no_exp;
	no_exp.setType("JWT");
	no_exp.setAlgorithm("HS256");
	no_exp.payload().set("prv", static_cast<int>(AuthClearance::BLACK));
	const auto no_exp_jwt{signer.sign(no_exp, Poco::JWT::Signer::ALGO_HS256)};
	REQUIRE_FALSE(jwt_grants_clearance(no_exp_jwt, k_secret, static_cast<int>(AuthClearance::PUBLIC)));
}
