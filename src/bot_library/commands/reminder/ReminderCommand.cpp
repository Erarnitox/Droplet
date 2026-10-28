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

#include "ReminderCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>
#include <message.h>
#include <queues.h>
#include <snowflake.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <atomic>
#include <format>

namespace {
// Reminders live only in the Discord library's timer queue: bound their number and
// duration so that nobody can grow it without limit.
constexpr long k_max_reminder_seconds{7L * 24 * 60 * 60};
constexpr int k_max_pending_reminders{500};
std::atomic<int> g_pending_reminders{0};
}  // namespace

//-----------------------------------------------------
//
//-----------------------------------------------------
void ReminderCommand::start_reminder(size_t channel_id,
									 dpp::snowflake user_id,
									 const std::string& user_mention,
									 const std::string& message,
									 size_t timestep_sec) {
	++g_pending_reminders;
	const dpp::timer_callback_t on_tick{[this, channel_id, user_id, user_mention, message](dpp::timer timer_handle) {
		// runs on the event-loop thread: must not throw
		try {
			const auto reminder_msg{
				std::format("**REMINDER:** {}\n{}", user_mention, Core::strip_broadcast_mentions(message))};
			dpp::message msg(channel_id, reminder_msg);
			msg.set_allowed_mentions(false, false, false, false, {user_id}, {});
			discord_.message_create(msg);
		} catch (...) {
		}
		discord_.stop_timer(timer_handle);
		--g_pending_reminders;
	}};

	discord_.start_timer(on_tick, timestep_sec);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
ReminderCommand::ReminderCommand(AppContext& ctx) : discord_(ctx.discord) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);

	this->command_options.emplace_back(
		dpp::co_integer, "time_seconds", "How long to wait until the reminder is sent? (In seconds)", true);

	this->command_options.emplace_back(dpp::co_string, "message", "The Message for the Reminder", true);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void ReminderCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	const auto& cmd{event.command};
	const auto time_variant{event.get_parameter("time_seconds")};
	const auto* const time_param{std::get_if<long>(&time_variant)};
	if (time_param == nullptr) {
		return;
	}
	const long time_sec{*time_param};

	const auto message{Core::get_parameter(discord_, event, "message")};
	if (message.empty()) {
		return;
	}

	if (time_sec > k_max_reminder_seconds) {
		Core::timed_reply_private(discord_, event, "Reminders can be set at most 7 days (604800 seconds) ahead!", 3000);
	} else if (g_pending_reminders.load() >= k_max_pending_reminders) {
		Core::timed_reply_private(discord_, event, "Too many pending reminders right now, try again later!", 3000);
	} else if (time_sec <= 0) {
		Core::timed_reply_private(discord_, event, std::format("Reminder from the past:\n{}", message), 2000);
	} else {
		start_reminder(cmd.get_channel().id, cmd.usr.id, cmd.usr.get_mention(), message, static_cast<size_t>(time_sec));
		Core::timed_reply_private(discord_, event, "Reminder set!", 2000);
	}
}
