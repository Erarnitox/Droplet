#include "SetChannelCommand.hpp"

#include <appcommand.h>
#include <fmt/core.h>

#include <Core.hpp>
#include <variant>

#include "IMemberCommand.hpp"

SetChannelCommand::SetChannelCommand() : IGlobalSlashCommand() {
	this->command_name = "set_channel";
	this->command_description = "Configure channels for bot events (Admin only!)";
	this->command_options.push_back(
		dpp::command_option(dpp::co_string, "type", "What should this channel be used for?", true)
			.add_choice(dpp::command_option_choice("Welcome Message Channel", std::string("channel_welcome")))
			.add_choice(dpp::command_option_choice("Goodbye Message Channel", std::string("channel_goodbye")))
			.add_choice(dpp::command_option_choice("Server Logging Channel", std::string("channel_log")))
		);
}

void SetChannelCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		event.reply(dpp::message("Only admins are allowed to use this command!").set_flags(dpp::m_ephemeral));
		return;
	}

	const auto type{Core::get_parameter(*Bot::ctx, event, "type")};
	if (type.empty()) {
		return;
	}

	if (type == "channel_welcome") {
		// TODO: Database::insert_welcome_channel_id(event.command.get_guild().id, event.command.get_channel().id);
		event.reply(dpp::message("Channel was set as welcome channel!").set_flags(dpp::m_ephemeral));
	} else if (type == "channel_goodbye") {
		// TODO: Database::insert_goodbye_channel_id(event.command.get_guild().id, event.command.get_channel().id);
		event.reply(dpp::message("Channel was set as goodbye channel!").set_flags(dpp::m_ephemeral));
	} else if (type == "channel_log") {
		// TODO: Database::insert_log_channel_id(event.command.get_guild().id, event.command.get_channel().id);
		event.reply(dpp::message("Channel was set as logging channel!").set_flags(dpp::m_ephemeral));
	}
}