/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RestApiServerApplication.hpp"

#include <exception>
#include <filesystem>
#include <iostream>

#include "Poco/Exception.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timespan.h"
#include "RestApiRequestHandlerFactory.hpp"

namespace {

// Bounded server parameters. Poco's defaults keep idle / half-open connections for a
// minute and allow unlimited keep-alive requests, which lets a handful of slow clients
// occupy every worker thread and make the REST API unresponsive.
Poco::Net::HTTPServerParams::Ptr make_server_params() {
	Poco::Net::HTTPServerParams::Ptr params{new Poco::Net::HTTPServerParams};
	params->setMaxThreads(16);
	params->setMaxQueued(64);
	params->setTimeout(Poco::Timespan(10, 0));
	params->setKeepAlive(false);
	params->setSoftwareVersion("droplet");
	return params;
}

}  // namespace

int RestApiServerApplication::main(const std::vector<std::string>& args) {
	(void)args;
	const uint16_t port{RestApi::port};

	try {
		if (RestApi::ssl_enabled) {
			const std::string certificateFile{"server.crt"};
			const std::string privateKeyFile{"server.key"};

			if (not std::filesystem::exists(certificateFile)) {
				std::cerr << "Error: " << certificateFile << " does not exist!" << '\n';
				return Poco::Util::Application::EXIT_IOERR;
			}

			if (not std::filesystem::exists(privateKeyFile)) {
				std::cerr << "Error: " << privateKeyFile << " does not exist!" << '\n';
				return Poco::Util::Application::EXIT_IOERR;
			}

			Poco::Net::initializeSSL();
			const std::string privateKeyPassphrase = "";
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
																	 privateKeyFile,
																	 certificateFile,
																	 privateKeyPassphrase,
																	 Poco::Net::Context::VERIFY_NONE);

			// Stack sockets: Poco HTTPServer duplicates the listener; pairing HTTPServer with the
			// socket in one scope avoids orphaned allocations reported by Valgrind.
			Poco::Net::SecureServerSocket socket(port, 64, context);
			Poco::Net::HTTPServer server(new RestApiRequestHandlerFactory(db_), socket, make_server_params());
			std::cout << "REST Server is running on https://localhost:" << port << '\n';
			server.start();
			waitForTerminationRequest();
			server.stop();
			Poco::Net::uninitializeSSL();
			return Poco::Util::Application::EXIT_OK;
		}

		Poco::Net::ServerSocket socket(port);
		Poco::Net::HTTPServer server(new RestApiRequestHandlerFactory(db_), socket, make_server_params());
		std::cout << "REST Server is running on http://localhost:" << port << '\n';
		server.start();
		waitForTerminationRequest();
		server.stop();
		return Poco::Util::Application::EXIT_OK;

	} catch (const Poco::Exception& e) {
		std::cerr << "REST server error: " << e.displayText() << '\n';
		if (RestApi::ssl_enabled) {
			Poco::Net::uninitializeSSL();
		}
		return Poco::Util::Application::EXIT_IOERR;
	} catch (const std::exception& e) {
		std::cerr << "REST server error: " << e.what() << '\n';
		if (RestApi::ssl_enabled) {
			Poco::Net::uninitializeSSL();
		}
		return Poco::Util::Application::EXIT_IOERR;
	}
}
