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

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include <AuthHandler.hpp>
#include <JWTUtils.hpp>
#include <Secrets.hpp>
#include <WebUserDTO.hpp>
#include <WebUserRepository.hpp>

#include <sstream>

#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

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

	if (declared_body_too_large(req, k_auth_body_max_bytes)) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
		resp.send() << R"({"error":"request_too_large"})";
		return;
	}

	if (not auth_rate_limiter().allow(http_client_key(req))) {
		resp.setStatus(static_cast<Poco::Net::HTTPResponse::HTTPStatus>(429));
		resp.setReason("Too Many Requests");
		resp.send() << R"({"error":"too_many_requests"})";
		return;
	}

	const auto body{read_limited(req.stream(), k_auth_body_max_bytes)};
	if (not body.has_value()) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_REQUEST_ENTITY_TOO_LARGE);
		resp.send() << R"({"error":"request_too_large"})";
		return;
	}

	std::istringstream body_stream{*body};
	const Poco::Net::HTMLForm form(req, body_stream);

	const auto grant = form.get("grant_type", "");
	if (grant != "password") {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		resp.send() << R"({"error":"unsupported_grant_type"})";
		return;
	}

	const std::string username = form.get("username", "");
	const std::string password = form.get("password", "");

	if (username.empty() || username.size() > k_username_max_length || password.empty() ||
		password.size() > k_password_max_length) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
		resp.send() << R"({"error":"invalid_grant"})";
		return;
	}

	WebUserRepository repo{db_};
	WebUserDTO user_dto = repo.get(username);

	if (user_dto.id == 0 || user_dto.username.empty() || not user_dto.is_verified ||
		not verifyPassword(password, user_dto.password)) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
		resp.send() << R"({"error":"invalid_grant"})";
		return;
	}

	if (!jwt_secret_is_strong(_secret)) {
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
		resp.send() << R"({"error":"server_misconfigured"})";
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
