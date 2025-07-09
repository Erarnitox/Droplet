#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>

#include <LeaderboardHandler.hpp>
#include <UserRepository.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
void LeaderboardHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
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
		Poco::URI uri(request.getURI());
		Poco::URI::QueryParameters params = uri.getQueryParameters();

		std::optional<std::string> category{std::nullopt};
		std::optional<std::vector<std::string>> tags{std::nullopt};

		UserRepository repo;
		auto users{Poco::JSON::Array()};

		for (const auto& user : repo.getTopTen()) {
			auto usr{Poco::JSON::Object()};
			usr.set("user_id", user.user_id);
			usr.set("user_name", user.user_name);
			usr.set("color", user.color);
			usr.set("exp", user.exp);

			users.add(usr);
		}

		responseJSON->set("users", users);
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