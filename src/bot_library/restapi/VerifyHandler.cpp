
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>

#include <UserManager.hpp>
#include <VerifyHandler.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
void VerifyHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
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

		std::optional<std::string> token{std::nullopt};

		for (const auto& param : params) {
			if (param.first == "token") {
				token = param.second;
				break;
			}
		}

		if (not token.has_value()) {
			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			responseJSON->set("status", "error");
			responseJSON->set("message", "Missing token");
			std::ostream& ostr = response.send();
			Poco::JSON::Stringifier::stringify(responseJSON, ostr);
			return;
		}

		if (UserManager::getInstance().verifyUser(token.value())) {
			response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
			responseJSON->set("status", "success");
			responseJSON->set("message", "Verification successful");
		} else {
			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			responseJSON->set("status", "error");
			responseJSON->set("message", "Invalid token");
		}

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
