/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RestApiRequestHandlerFactory.hpp"

#include <AuthHandler.hpp>

#include "LeaderboardHandler.hpp"
#include "NotFoundHandler.hpp"
#include "Poco/Net/HTTPRequest.h"
#include "RegistrationHandler.hpp"
#include "ResQueryHandler.hpp"
#include "Secrets.hpp"
#include "StatusHandler.hpp"
#include "VerifyHandler.hpp"

Poco::Net::HTTPRequestHandler* RestApiRequestHandlerFactory::createRequestHandler(
	const Poco::Net::HTTPServerRequest& request) {
	const auto& uri{request.getURI()};

	if (uri.starts_with("/status")) {
		return new StatusHandler;
	}
	if (uri.starts_with("/auth")) {
		const Secrets& sec{Secrets::getInstance()};
		return new AuthHandler(sec.getSecret("jwt_secret"), "erarnitox.de");
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
		return new ResQueryHandler;
	}
	if ((request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET ||
		 request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) &&
		uri.starts_with("/leaderboard")) {
		return new LeaderboardHandler;
	}
	return new NotFoundHandler;
}
