#include <Poco/Base64Encoder.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JWT/Token.h>

#include <JWTUtils.hpp>
#include <ctime>

#include "Poco/Timestamp.h"

std::string JWTUtils::generateToken(const Poco::JSON::Object& claims,
									const std::string& secret,
									const std::string& issuer) {
	Poco::JWT::Token token;
	token.setType("JWT");
	token.setAlgorithm("HS256");
	token.setIssuedAt(Poco::Timestamp());
	token.setIssuer(issuer);

	for (const auto& claim : claims) {
		token.payload().set(claim.first, claim.second);
	}

	Poco::JWT::Signer signer(secret);
	return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
}