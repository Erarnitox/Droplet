/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RestApi.hpp"

#include "RestApiServerApplication.hpp"

const bool RestApi::ssl_enabled{true};
const unsigned short RestApi::port{3000};

void RestApi::start() {
	RestApiServerApplication app;
	int argc = 1;
	char* argv[] = {const_cast<char*>("rest_server")};
	int result = app.run(argc, argv);
	(void)result;
}
