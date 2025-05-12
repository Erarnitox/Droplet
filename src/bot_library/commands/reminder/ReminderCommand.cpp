#include "ReminderCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>
#include <message.h>
#include <queues.h>
#include <snowflake.h>

#include <Core.hpp>
#include <format>

static inline auto start_reminder(size_t channel_id,
								  const std::string& user,
								  const std::string& message,
								  size_t timestep_sec = 500) -> void {
	const dpp::timer_callback_t on_tick{[channel_id, user, message](dpp::timer timer_handle) {
		const auto reminder_msg{std::format("**REMINDER:** {}\n{}", user, message)};
		dpp::message msg(channel_id, reminder_msg);
		msg.set_allowed_mentions(true, false, false, true);
		Bot::ctx->message_create(msg);
		Bot::ctx->stop_timer(timer_handle);
	}};

	Bot::ctx->start_timer(on_tick, timestep_sec);
}

ReminderCommand::ReminderCommand() : IGlobalSlashCommand() {
	this->command_name = "reminder";
	this->command_description = "Set a reminder for yourself!";

	this->command_options.emplace_back(
		dpp::co_integer, "time_seconds", "How long to wait until the reminder is sent? (In seconds)", true);

	this->command_options.emplace_back(dpp::co_string, "message", "The Message for the Reminder", true);
}

void ReminderCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	const auto& cmd{event.command};
	const auto time_sec{std::get<long>(event.get_parameter("time_seconds"))};

	const auto message{Core::get_parameter(*Bot::ctx, event, "message")};
	if (message.empty()) {
		return;
	}

	if(time_sec <= 0) {
		Core::timed_reply_private(*Bot::ctx, event, std::format("Reminder from the past:\n{}", message), 2000);
	} else {
		start_reminder(cmd.get_channel().id, cmd.usr.get_mention(), message, static_cast<size_t>(time_sec));
		Core::timed_reply_private(*Bot::ctx, event, "Reminder set!", 2000);
	}
}
