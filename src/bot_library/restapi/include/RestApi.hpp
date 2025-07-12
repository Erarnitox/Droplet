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
 *  Documentation: https://droplet.erarnitox.de/doxygen/bot_library
 */

#pragma once

#include <string>

//-----------------------------------------------------
//
//-----------------------------------------------------
class RestApi {
  public:
	const static ushort port;
	const static bool ssl_enabled;
	static auto start() -> void;
};