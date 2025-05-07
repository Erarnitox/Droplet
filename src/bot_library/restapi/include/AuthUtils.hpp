#pragma once

#include "Poco/HexBinaryEncoder.h"
#include "Poco/JWT/Signer.h"
#include "Poco/JWT/Token.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/SHA2Engine.h"

//-----------------------------------------------------
//
//-----------------------------------------------------
static inline std::string hashPassword(const std::string& password) {
	Poco::SHA2Engine sha256;
	sha256.update(password);
	const Poco::DigestEngine::Digest& digest = sha256.digest();

	std::stringstream ss;
	Poco::HexBinaryEncoder hexEncoder(ss);
	hexEncoder.write(reinterpret_cast<const char*>(&digest[0]), static_cast<long>(digest.size()));
	hexEncoder.close();

	return ss.str();
}

//-----------------------------------------------------
//
//-----------------------------------------------------
static inline bool verifyPassword(const std::string& password, const std::string& storedHash) {
	return hashPassword(password) == storedHash;
}

//-----------------------------------------------------
//
//-----------------------------------------------------
enum AuthClearance { NONE = 0, PUBLIC = 10, PRIVATE = 100, SECRET = 1000, TOP_SECRET = 10000, BLACK = 100000 };

//-----------------------------------------------------
//
//-----------------------------------------------------
struct AuthUtil {
	// Returns { sub, roles } on success; empty map on failure
	[[nodiscard]]
	static inline bool is_authorized(const Poco::Net::HTTPServerRequest& req,
									 AuthClearance minClearance = AuthClearance::SECRET) {
		if (minClearance == AuthClearance::NONE)
			return true;  // no login required
		if (not req.has("Authorization"))
			return false;  // not logged in

		const auto auth = req.get("Authorization");
		const std::string bearer = "Bearer ";

		if (auth.rfind(bearer, 0) != 0)
			return false;

		// extract the token
		const std::string jwt = auth.substr(bearer.size());

		try {
			Poco::JWT::Signer signer("<secret>");
			Poco::JWT::Token token;
			signer.tryVerify(jwt, token);

			const auto& claims{token.payload()};
			for (const auto& claim : claims) {
				const auto& key{claim.first};
				const auto& value{claim.second};

				if (key == "prv") {
					if (value.convert<int>() >= minClearance) {
						return true;
					} else {
						return false;
					}
				}
			}
		} catch (...) {
			return false;
		}
		return false;
	}
};