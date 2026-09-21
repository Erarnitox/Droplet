#pragma once

#include <dpp/cluster.h>

#include "CommandTypes.hpp"

void register_global_slash_commands_on_ready(dpp::cluster& ctx, const slash_commands_t& slash_commands);
