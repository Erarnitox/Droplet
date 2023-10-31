#pragma once

#include <dpp/cluster.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>

#include <fstream>

auto start_bot() -> void;

[[nodiscard]] static inline auto read_bot_token(const std::string& file) -> std::string;

[[nodiscard]] static inline auto read_database_credentials(const std::string& file) -> std::string;