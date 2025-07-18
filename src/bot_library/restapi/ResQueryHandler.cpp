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

#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>

#include <ResQueryHandler.hpp>
#include <ResourceRepository.hpp>
#include <optional>

#include "Poco/JSON/Stringifier.h"
#include "ResourceDTO.hpp"

//-----------------------------------------------------
//
//-----------------------------------------------------
void ResQueryHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
	// Set CORS headers for all responses
	response.set("Access-Control-Allow-Origin", "https://droplet.erarnitox.de");
	response.set("Access-Control-Allow-Methods", "GET, OPTIONS");
	response.set("Access-Control-Allow-Headers", "Content-Type, Accept");
	response.set("Access-Control-Allow-Credentials", "true");

	// Handle pre-flight OPTIONS request
	if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_OPTIONS) {
		response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
		response.send();  // Send empty response for OPTIONS
		return;
	}

	response.setContentType("application/json");
	Poco::JSON::Object::Ptr responseJSON = new Poco::JSON::Object();

	if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_GET) {
		response.setStatus(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		responseJSON->set("status", "error");
		responseJSON->set("message", "Method Not Allowed");
		std::ostream& ostr = response.send();
		Poco::JSON::Stringifier::stringify(responseJSON, ostr);
		return;
	}

	try {
		const Poco::URI uri(request.getURI());
		const Poco::URI::QueryParameters params = uri.getQueryParameters();

		std::optional<std::string> category{std::nullopt};
		std::optional<std::vector<std::string>> tags{std::nullopt};

		for (const auto& param : params) {
			if (param.first == "category") {
				category = param.second;
			}
		}

		// tags and difficulty get filtered client side

		ResourceRepository repo;
		auto resources{Poco::JSON::Array()};

		for (const auto& res : repo.get(category.value_or("all"))) {
			auto resource{Poco::JSON::Object()};
			resource.set("title", res.title);
			resource.set("description", res.description);
			resource.set("category", res.category);
			resource.set("difficulty", res.difficulty);
			resource.set("creator", res.creator);
			resource.set("creator_id", res.creator_id);
			resource.set("url", res.url);
			resource.set("guild_id", res.guild_id);
			resource.set("tags", res.tags);
			resource.set("id", res.id);

			resources.add(resource);
		}

		responseJSON->set("resources", resources);
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