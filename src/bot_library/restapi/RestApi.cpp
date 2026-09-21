/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "RestApi.hpp"

#include "AuthUtils.hpp"
#include "DatabaseExecutor.hpp"
#include "RestApiServerApplication.hpp"
#include "Secrets.hpp"
#include "UserManager.hpp"

const bool RestApi::ssl_enabled{true};
const unsigned short RestApi::port{3000};

void RestApi::start(DatabaseExecutor& db) {
	(void)Secrets::getInstance().requireSecret("jwt_secret", k_jwt_secret_min_bytes);
	UserManager::getInstance().set_executor(db);
	RestApiServerApplication app{db};
	int argc = 1;
	std::string argv0{"rest_server"};
	char* argv[] = {argv0.data()};
	int result = app.run(argc, argv);
	(void)result;
}
