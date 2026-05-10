/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "StatusHandler.hpp"

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <ostream>

void StatusHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
	(void)request;
	response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
	response.setContentType("application/json");
	std::ostream& out = response.send();

	Poco::JSON::Object json;
	json.set("Status", "OK");
	json.set("uri", request.getURI());

	Poco::JSON::Stringifier::stringify(json, out);
}
