/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <Bot.hpp>

/**
 * Registers slash commands with Discord when the cluster becomes ready (checksum / bulk sync).
 */
void register_global_slash_commands_on_ready(ctx_t& ctx, const slash_commands_t& slash_commands);
