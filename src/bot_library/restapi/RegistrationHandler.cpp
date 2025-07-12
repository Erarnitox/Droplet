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
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>

#include <RegistrationHandler.hpp>
#include <UserManager.hpp>

#include "Poco/JSON/Parser.h"

//-----------------------------------------------------
//
//-----------------------------------------------------
void RegistrationHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
										Poco::Net::HTTPServerResponse& response) {
	response.setContentType("application/json");
	Poco::JSON::Object::Ptr responseJSON = new Poco::JSON::Object();

	if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST) {
		response.setStatus(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		responseJSON->set("status", "error");
		responseJSON->set("message", "Method Not Allowed");
		std::ostream& ostr = response.send();
		Poco::JSON::Stringifier::stringify(responseJSON, ostr);
		return;
	}

	if (request.getContentType() != "application/json") {
		response.setStatus(Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE);
		Poco::JSON::Object::Ptr resp = new Poco::JSON::Object();
		resp->set("status", "error");
		resp->set("message", "Content-Type must be application/json");
		std::ostream& ostr = response.send();
		Poco::JSON::Stringifier::stringify(resp, ostr);
		return;
	}

	try {
		std::string body;
		Poco::StreamCopier::copyToString(request.stream(), body);

		Poco::JSON::Parser parser;
		Poco::Dynamic::Var result = parser.parse(body);

		Poco::JSON::Object::Ptr json = result.extract<Poco::JSON::Object::Ptr>();

		if (not json->has("username") || not json->has("email") || not json->has("password")) {
			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			Poco::JSON::Object::Ptr resp = new Poco::JSON::Object();
			resp->set("status", "error");
			resp->set("message", "Missing JSON fields");
			std::ostream& ostr = response.send();
			Poco::JSON::Stringifier::stringify(resp, ostr);
			return;
		}

		const std::string username = json->getValue<std::string>("username");
		const std::string email = json->getValue<std::string>("email");
		const std::string password = json->getValue<std::string>("password");

		UserManager::getInstance().addUser(email, username, password);

		response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
		responseJSON->set("status", "success");
		responseJSON->set("message", "User added, please check your email for verification");
		std::ostream& ostr = response.send();
		Poco::JSON::Stringifier::stringify(responseJSON, ostr);
	} catch (const std::exception& e) {
		response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		responseJSON->set("status", "error");
		responseJSON->set("message", std::string("Error: ") + e.what());
		std::ostream& ostr = response.send();
		Poco::JSON::Stringifier::stringify(responseJSON, ostr);
	}
}
