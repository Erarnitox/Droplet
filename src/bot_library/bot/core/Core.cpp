/*
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: This class offers access to all the
 * 	core functionality and utility needed by the bot
 *
 *  Documentation: https://droplet.erarnitox.de/doxygen/html/core
 */

#include <dpp/permissions.h>
#include <dpp/timer.h>

#include <Core.hpp>
#include <chrono>
#include <ratio>
#include <regex>
#include <thread>
#include <variant>
#include <vector>

/**
 * @brief checks if a guild member is admin of that guild
 *
 * @param member the guild member
 * @return whether the member has admin right on guild or not
 */
bool Core::is_admin(const dpp::guild_member& member) noexcept {
	for (const auto& role_id : member.get_roles()) {
		const dpp::role& role{*dpp::find_role(role_id)};
		if (role.has_administrator())
			return true;
	}
	return false;
}

/**
 * @brief gets the id of a mentioned role
 *
 * @param mention the mention string of a role
 * @return returns the role id as a std::string
 */
std::string Core::get_role_id(const std::string& mention) noexcept {
	std::regex re("<@&([0-9]+)>");
	std::smatch match;

	if (std::regex_search(mention, match, re)) {
		return match.str(1);
	} else {
		return "";
	}
}

/**
 * @brief get the id of a channel from a mention
 *
 * @param mention the mention string of a channel
 * @return the channel id as a std::string
 */
std::string Core::get_channel_id(const std::string& mention) noexcept {
	std::regex re("<#([0-9]+)>");
	std::smatch match;

	if (std::regex_search(mention, match, re)) {
		return match.str(1);
	} else {
		return "";
	}
}

/**
 * @brief template for timed replies
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the event to be replied to
 * @param message the text to be put into the reply message
 * @param time_mills the time to display the reply in milliseconds
 * @return doesn't return
 */
template <typename CMD_TYPE>
void timed_reply_template(dpp::cluster& bot,
						  const CMD_TYPE event,
						  const std::string& message,
						  size_t time_mills) noexcept {
	event.reply(message);

	dpp::timer_callback_t on_tick = [&bot, event](dpp::timer deleteTimer) {
		event.delete_original_response();
		bot.stop_timer(deleteTimer);
	};

	bot.start_timer(on_tick, time_mills / 1000);
}

/**
 * @brief replies to an event and displays the reply for a limited time
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the event to be replied to
 * @param message the text to be put into the reply message
 * @param time_mills the time to display the reply in milliseconds
 * @return doesn't return
 */
void Core::timed_reply(dpp::cluster& bot,
					   const dpp::slashcommand_t event,
					   const std::string& message,
					   size_t time_mills) noexcept {
	timed_reply_template<dpp::slashcommand_t>(bot, event, message, time_mills);
}

/**
 * @brief replies to an event and displays the reply for a limited time
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the event to be replied to
 * @param message the text to be put into the reply message
 * @param time_mills the time to display the reply in milliseconds
 * @return doesn't return
 */
void Core::timed_reply(dpp::cluster& bot,
					   const dpp::form_submit_t event,
					   const std::string& message,
					   size_t time_mills) noexcept {
	timed_reply_template<dpp::form_submit_t>(bot, event, message, time_mills);
}

/**
 * @brief template for private timed replies
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the event to be replied to
 * @param message the text to be put into the reply message
 * @param time_mills the time to display the reply in milliseconds
 * @return doesn't return
 */
template <typename CMD_TYPE>
void timed_reply_private_template(dpp::cluster& bot,
								  const CMD_TYPE event,
								  const std::string& message,
								  size_t time_mills) noexcept {
	event.reply(dpp::message(message).set_flags(dpp::m_ephemeral));

	dpp::timer_callback_t on_tick = [&bot, event](dpp::timer deleteTimer) {
		event.delete_original_response();
		bot.stop_timer(deleteTimer);
	};

	bot.start_timer(on_tick, time_mills / 1000);
}

/**
 * @brief replies to an event and displays the reply for a limited time
 * and only to the sender of the event
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the event to be replied to
 * @param message the text to be put into the reply message
 * @param time_mills the time to display the reply in milliseconds
 * @return doesn't return
 */
void Core::timed_reply_private(dpp::cluster& bot,
							   const dpp::slashcommand_t event,
							   const std::string& message,
							   size_t time_mills) noexcept {
	timed_reply_private_template<dpp::slashcommand_t>(bot, event, message, time_mills);
}

/**
 * @brief replies to an event and displays the reply for a limited time
 * and only to the sender of the event
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the event to be replied to
 * @param message the text to be put into the reply message
 * @param time_mills the time to display the reply in milliseconds
 * @return doesn't return
 */
void Core::timed_reply_private(dpp::cluster& bot,
							   const dpp::form_submit_t event,
							   const std::string& message,
							   size_t time_mills) noexcept {
	timed_reply_private_template<dpp::form_submit_t>(bot, event, message, time_mills);
}

/**
 * @brief get the parameter from a slash command
 *
 * @param bot the cluster that can be thought of the current bot instance
 * @param event the slash command event
 * @param name the name of the parameter
 * @return the parameter with the given name as a std::string
 */
std::string Core::get_parameter(dpp::cluster& bot, const dpp::slashcommand_t event, const std::string& name) noexcept {
	const auto variant{event.get_parameter(name)};
	const auto value_ptr{std::get_if<std::string>(&variant)};

	if (!value_ptr) {
		Core::timed_reply_private(
			bot, event, std::format("Could not retrieve parameter '{}' from the Message!", name), 3000);

		return std::string("");
	}
	return *value_ptr;
}