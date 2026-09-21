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

#include <Poco/JWT/Signer.h>
#include <Poco/JWT/Token.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include <AuthUtils.hpp>
#include <JWTUtils.hpp>
#include <stdexcept>

//-----------------------------------------------------
//
//-----------------------------------------------------
std::string JWTUtils::generateToken(const Poco::JSON::Object& claims,
									const std::string& secret,
									const std::string& issuer) {
	if (!jwt_secret_is_strong(secret)) {
		throw std::invalid_argument("jwt secret must be at least 32 bytes");
	}

	Poco::JWT::Token token;
	token.setType("JWT");
	token.setAlgorithm("HS256");
	token.setIssuedAt(Poco::Timestamp());
	token.setExpiration(Poco::Timestamp() + Poco::Timespan(3600, 0));
	token.setIssuer(issuer);

	for (const auto& claim : claims) {
		token.payload().set(claim.first, claim.second);
	}

	const Poco::JWT::Signer signer(secret);
	return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
}