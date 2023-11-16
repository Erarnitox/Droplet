#include "SetChannelCommand.hpp"

#include <appcommand.h>
#include <fmt/core.h>

#include <Core.hpp>
#include <GoodbyeChannelRepository.hpp>
#include <LogChannelRepository.hpp>
#include <WelcomeChannelRepository.hpp>
#include <variant>

#include "WelcomeChannelDTO.hpp"

SetChannelCommand::SetChannelCommand() : IGlobalSlashCommand() {
	this->command_name = "set_channel";
	this->command_description = "Configure channels for bot events (Admin only!)";
	this->command_options.push_back(
		dpp::command_option(dpp::co_string, "type", "What should this channel be used for?", true)
			.add_choice(dpp::command_option_choice("Welcome Message Channel", std::string("channel_welcome")))
			.add_choice(dpp::command_option_choice("Goodbye Message Channel", std::string("channel_goodbye")))
			.add_choice(dpp::command_option_choice("Server Logging Channel", std::string("channel_log"))));
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

	const auto& cmd{event.command};

	if (type == "channel_welcome") {
		WelcomeChannelRepository repo;
		WelcomeChannelDTO data{cmd.get_guild().id, cmd.get_channel().id};

		if (repo.create(data)) {
			event.reply(dpp::message("Channel was set as welcome channel!").set_flags(dpp::m_ephemeral));
		} else if (repo.update(data)) {
			event.reply(dpp::message("Welcome channel was updated!").set_flags(dpp::m_ephemeral));
		} else {
			event.reply(dpp::message("Error: Welcome channel can't be saved to Database!").set_flags(dpp::m_ephemeral));
		}

	} else if (type == "channel_goodbye") {
		GoodbyeChannelRepository repo;
		GoodbyeChannelDTO data{cmd.get_guild().id, cmd.get_channel().id};

		if (repo.create(data)) {
			event.reply(dpp::message("Channel was set as goodbye channel!").set_flags(dpp::m_ephemeral));
		} else if (repo.update(data)) {
			event.reply(dpp::message("Goodbye channel was updated!").set_flags(dpp::m_ephemeral));
		} else {
			event.reply(dpp::message("Error: Goodbye channel can't be saved to Database!").set_flags(dpp::m_ephemeral));
		}

	} else if (type == "channel_log") {
		LogChannelRepository repo;
		LogChannelDTO data{cmd.get_guild().id, cmd.get_channel().id};

		if (repo.create(data)) {
			event.reply(dpp::message("Channel was set as log channel!").set_flags(dpp::m_ephemeral));
		} else if (repo.update(data)) {
			event.reply(dpp::message("Log channel was updated!").set_flags(dpp::m_ephemeral));
		} else {
			event.reply(dpp::message("Error: Log channel can't be saved to Database!").set_flags(dpp::m_ephemeral));
		}
	}
}