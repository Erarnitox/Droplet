/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RestApiServerApplication.hpp"

#include "RestApiRequestHandlerFactory.hpp"

#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SSLManager.h"

#include <filesystem>
#include <iostream>

using namespace Poco::Net;
using namespace Poco::Util;

int RestApiServerApplication::main(const std::vector<std::string>& args) {
	(void)args;
	const uint16_t port{RestApi::port};

	std::unique_ptr<ServerSocket> pSocket;

	try {
		if constexpr (RestApi::ssl_enabled) {
			const std::string certificateFile{"server.crt"};
			const std::string privateKeyFile{"server.key"};

			if (not std::filesystem::exists(certificateFile)) {
				std::cerr << "Error: " << certificateFile << " does not exist!" << std::endl;
				return Application::EXIT_IOERR;
			}

			if (not std::filesystem::exists(privateKeyFile)) {
				std::cerr << "Error: " << privateKeyFile << " does not exist!" << std::endl;
				return Application::EXIT_IOERR;
			}

			Poco::Net::initializeSSL();
			const std::string privateKeyPassphrase = "";
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
																	   privateKeyFile,
																	   certificateFile,
																	   privateKeyPassphrase,
																	   Poco::Net::Context::VERIFY_NONE);

			pSocket = std::make_unique<SecureServerSocket>(port, 64, context);
		} else {
			pSocket = std::make_unique<ServerSocket>(port);
		}
	} catch (const Poco::Exception& e) {
		std::cout << "Error: " << e.displayText() << std::endl;
		return Application::EXIT_IOERR;
	}

	HTTPServer server(new RestApiRequestHandlerFactory, *pSocket, new HTTPServerParams);
	std::cout << "REST Server is running on " << (RestApi::ssl_enabled ? "https" : "http") << "://localhost:" << port
			  << std::endl;
	server.start();

	waitForTerminationRequest();
	server.stop();

	if constexpr (RestApi::ssl_enabled) {
		Poco::Net::uninitializeSSL();
	}

	return Application::EXIT_OK;
}
