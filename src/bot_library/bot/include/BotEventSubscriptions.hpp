/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#pragma once

#include <Bot.hpp>

/**
 * Subscribes all DPP event handlers that dispatch to registered command lists on Bot.
 */
void subscribe_bot_event_handlers(ctx_t& ctx);
