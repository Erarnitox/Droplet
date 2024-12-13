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

auto start_bot(bool IS_TEST = false) -> void;

[[nodiscard]] static std::string read_bot_token(const std::string& file);

[[nodiscard]] static std::string read_database_credentials(const std::string& file);