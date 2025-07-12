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

#pragma once

#include <Poco/SingletonHolder.h>

#include <string>

//-----------------------------------------------------
//
//-----------------------------------------------------
class UserManager {
  private:
	Poco::Mutex mutex;
	std::string smtpServer;
	std::string smtpUser;
	std::string smtpPassword;

	UserManager();

  public:
	static UserManager& getInstance() noexcept;
	void addUser(const std::string& email, const std::string& username, const std::string& password);
	bool verifyUser(const std::string& token);

  private:
	void sendVerificationEmail(const std::string& email, const std::string& token);
};