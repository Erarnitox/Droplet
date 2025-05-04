#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include <AuthHandler.hpp>
#include <JWTUtils.hpp>
#include <Secrets.hpp>
#include <WebUserDTO.hpp>
#include <WebUserRepository.hpp>

#include "Poco/HexBinaryEncoder.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
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
void AuthHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) {
	resp.setContentType("application/json");

	if (req.getMethod() != Poco::Net::HTTPRequest::HTTP_POST) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		resp.send() << R"({"error":"method_not_allowed"})";
		return;
	}

	Poco::Net::HTMLForm form(req, req.stream());

	auto grant = form.get("grant_type", "");
	if (grant != "password") {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		resp.send() << R"({"error":"unsupported_grant_type"})";
		return;
	}

	const std::string username = form.get("username", "");
	const std::string password = form.get("password", "");

	WebUserRepository repo;
	WebUserDTO user_dto = repo.get(username);

	if (not verifyPassword(password, user_dto.password)) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
		resp.send() << R"({"error":"invalid_grant"})";
		return;
	}

	// Build JWT claims
	Poco::JSON::Object claims;
	claims.set("sub", user_dto.username);
	claims.set("scope", "read write");

	// private claims
	claims.set("uid", user_dto.id);
	claims.set("prv", user_dto.clearance);

	std::string token = JWTUtils::generateToken(claims, _secret, _issuer);

	// Build token response
	Poco::JSON::Object jsonResp;
	jsonResp.set("access_token", token);
	jsonResp.set("token_type", "Bearer");
	jsonResp.set("scope", "read write");

	resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
	jsonResp.stringify(resp.send());
}