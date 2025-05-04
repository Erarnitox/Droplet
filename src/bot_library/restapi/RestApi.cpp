/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This Library combines all the
 * 	fuctionality of the bot into a static library
 * 	that can be used by the main binary and also
 * 	the unit_test binary.
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/index.html
 */

#include "RestApi.hpp"

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTTPResponse.h>

#include <AuthHandler.hpp>
#include <JWTUtils.hpp>
#include <UserRepository.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "Poco/JSON/Parser.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Util/ServerApplication.h"
#include "Secrets.hpp"
#include "UserDTO.hpp"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco;

const bool RestApi::ssl_enabled{true};
const ushort RestApi::port{3000};

//-----------------------------------------------------
//
//-----------------------------------------------------
class StatusHandler : public HTTPRequestHandler {
  public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
		(void)request;
		response.setStatus(HTTPResponse::HTTP_OK);
		response.setContentType("application/json");
		std::ostream& out = response.send();

		Poco::JSON::Object json;
		json.set("Status", "OK");
		json.set("uri", request.getURI());

		JSON::Stringifier::stringify(json, out);
	}
};

//-----------------------------------------------------
//
//-----------------------------------------------------
class MyRequestHandlerFactory : public HTTPRequestHandlerFactory {
  public:
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override {
		const auto& uri{request.getURI()};

		if (uri.starts_with("/status")) {
			return new StatusHandler;
		} else if (uri.starts_with("/auth")) {
			const Secrets& sec{Secrets::getInstance()};
			return new AuthHandler(sec.getSecret("jwt_secret"), "erarnitox.de");
		} else {
			return nullptr;
		}
	}
};

//-----------------------------------------------------
//
//-----------------------------------------------------
class HttpsServerApp : public ServerApplication {
  protected:
	int main(const std::vector<std::string>& args) override {
		(void)args;
		const uint16_t port{RestApi::port};

		std::unique_ptr<ServerSocket> pSocket;

		try {
			if constexpr (RestApi::ssl_enabled) {
				const std::string certificateFile{"server.crt"};
				const std::string privateKeyFile{"server.key"};

				if (!std::filesystem::exists(certificateFile)) {
					std::cerr << "Error: " << certificateFile << " does not exist!" << std::endl;
					return Application::EXIT_IOERR;
				}
				if (!std::filesystem::exists(privateKeyFile)) {
					std::cerr << "Error: " << privateKeyFile << " does not exist!" << std::endl;
					return Application::EXIT_IOERR;
				}

				// Initialize SSL
				Poco::Net::initializeSSL();
				const std::string privateKeyPassphrase = "";  // Set if your key is protected
				Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
																		 privateKeyFile,
																		 certificateFile,
																		 privateKeyPassphrase,
																		 Poco::Net::Context::VERIFY_NONE);

				pSocket = std::make_unique<SecureServerSocket>(port, 64, context);
			} else {
				pSocket = std::make_unique<ServerSocket>(port);
			}
		} catch (Poco::Exception& e) {
			std::cout << "Error: " << e.displayText() << std::endl;
			return Application::EXIT_IOERR;
		}

		// Create the HTTPServer instance using the chosen socket.
		HTTPServer server(new MyRequestHandlerFactory, *pSocket, new HTTPServerParams);
		std::cout << "REST Server is running on " << (RestApi::ssl_enabled ? "https" : "http")
				  << "://localhost:" << port << std::endl;
		server.start();

		waitForTerminationRequest();
		server.stop();

		if constexpr (RestApi::ssl_enabled) {
			Poco::Net::uninitializeSSL();
		}

		return Application::EXIT_OK;
	}
};

/**
 * @brief this is the entry point of the binary that will start the bot
 * @return doesn't return anything
 */
void RestApi::start() {
	HttpsServerApp app;
	int argc = 1;
	char* argv[] = {const_cast<char*>("rest_server")};
	int result = app.run(argc, argv);
	(void)result;

	return;
}