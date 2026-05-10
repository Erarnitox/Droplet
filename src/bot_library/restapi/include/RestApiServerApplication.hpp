/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include "RestApi.hpp"

#include "Poco/Util/ServerApplication.h"

/**
 * Poco ServerApplication that binds the HTTPS (or HTTP) REST listener for Droplet.
 */
class RestApiServerApplication final : public Poco::Util::ServerApplication {
  protected:
	int main(const std::vector<std::string>& args) override;
};
