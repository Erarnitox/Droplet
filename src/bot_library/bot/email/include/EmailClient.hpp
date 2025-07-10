/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: A simple email client to send emails from the bot
 *  using an external SMTP server
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/database
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

//-----------------------------------------------------
//
//-----------------------------------------------------
class EmailClient {
  public:
	EmailClient(const std::string& host, uint16_t port, const std::string& username, const std::string& password);
	EmailClient(const EmailClient&) = delete;
	EmailClient& operator=(const EmailClient&) = delete;
	~EmailClient() noexcept;

	void send(std::string_view from,
			  const std::vector<std::string>& to,
			  std::string_view subject,
			  std::string_view body);

  private:
	std::string _host;
	uint16_t _port;
	std::string _username;
	std::string _password;
};