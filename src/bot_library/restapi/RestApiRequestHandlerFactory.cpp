/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RestApiRequestHandlerFactory.hpp"

#include <AuthHandler.hpp>
#include <exception>
#include <iostream>
#include <memory>

#include "LeaderboardHandler.hpp"
#include "NotFoundHandler.hpp"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "RegistrationHandler.hpp"
#include "ResQueryHandler.hpp"
#include "Secrets.hpp"
#include "StatusHandler.hpp"
#include "VerifyHandler.hpp"

namespace {

/**
 * Decorator for every REST handler: an exception escaping from a handler (database error,
 * bad_alloc, malformed input reaching a library, ...) becomes a JSON 500 for that one request
 * instead of unwinding into the server's worker thread. Also adds basic hardening headers.
 */
class SafeHandler final : public Poco::Net::HTTPRequestHandler {
  public:
	explicit SafeHandler(Poco::Net::HTTPRequestHandler* inner) : inner_(inner) {
	}

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override {
		response.set("X-Content-Type-Options", "nosniff");
		try {
			inner_->handleRequest(request, response);
		} catch (const std::exception& ex) {
			report(response, ex.what());
		} catch (...) {
			report(response, "unknown exception");
		}
	}

  private:
	static void report(Poco::Net::HTTPServerResponse& response, const char* what) noexcept {
		try {
			std::cerr << "REST handler failed: " << what << '\n';
			if (not response.sent()) {
				response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
				response.setContentType("application/json");
				response.send() << R"({"status":"error","message":"internal_error"})";
			}
		} catch (...) {
			// the connection is gone; nothing left to do
		}
	}

	std::unique_ptr<Poco::Net::HTTPRequestHandler> inner_;
};

Poco::Net::HTTPRequestHandler* select_handler(const Poco::Net::HTTPServerRequest& request, DatabaseExecutor& db_);

}  // namespace

namespace {

Poco::Net::HTTPRequestHandler* select_handler(const Poco::Net::HTTPServerRequest& request, DatabaseExecutor& db_) {
	const auto& uri{request.getURI()};

	if (uri.starts_with("/status")) {
		return new StatusHandler;
	}
	if (uri.starts_with("/auth")) {
		const Secrets& sec{Secrets::getInstance()};
		return new AuthHandler(sec.getSecret("jwt_secret"), "erarnitox.de", db_);
	}
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && uri.starts_with("/register")) {
		return new RegistrationHandler;
	}
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET && uri.starts_with("/verify")) {
		return new VerifyHandler;
	}
	if ((request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET ||
		 request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) &&
		uri.starts_with("/resources")) {
		return new ResQueryHandler(db_);
	}
	if ((request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET ||
		 request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) &&
		uri.starts_with("/leaderboard")) {
		return new LeaderboardHandler(db_);
	}
	return new NotFoundHandler;
}

}  // namespace

Poco::Net::HTTPRequestHandler* RestApiRequestHandlerFactory::createRequestHandler(
	const Poco::Net::HTTPServerRequest& request) {
	try {
		return new SafeHandler(select_handler(request, db_));
	} catch (const std::exception& ex) {
		std::cerr << "REST routing failed: " << ex.what() << '\n';
		return new NotFoundHandler;
	}
}
