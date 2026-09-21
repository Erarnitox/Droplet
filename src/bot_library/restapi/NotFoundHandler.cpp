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
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <NotFoundHandler.hpp>
#include <format>

//----------------------------------------
//
//----------------------------------------
void NotFoundHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
	response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
	response.setContentType("application/json");

	Poco::JSON::Object::Ptr responseJSON = new Poco::JSON::Object();
	responseJSON->set("status", "error");
	responseJSON->set("message", std::format("Not found: [{}]", request.getURI()));

	std::ostream& ostr = response.send();
	Poco::JSON::Stringifier::stringify(responseJSON, ostr);
}
