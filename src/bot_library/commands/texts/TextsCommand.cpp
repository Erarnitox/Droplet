#include "TextsCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <Core.hpp>
#include <TextRepository.hpp>

TextsCommand::TextsCommand() : IGlobalSlashCommand() {
	this->command_name = "publish_ezine";
	this->command_description = "Add a pdf to erarnitox.de/pub (Erarnitox only!)";

	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "title", "The title of the E-Zine", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "description", "Document description", true));

	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "url", "Link to the document", true));
	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "author", "Author(s) of the document", true));
}

void TextsCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_erarnitox_admin(event.command.member)) {
		event.reply("Only Erarnitox's admins are allowed to run this command!");
		return;
	}

	const auto title{Core::get_parameter(*Bot::ctx, event, "title")};
	const auto description{Core::get_parameter(*Bot::ctx, event, "description")};
	const auto url{Core::get_parameter(*Bot::ctx, event, "url")};
	const auto author{Core::get_parameter(*Bot::ctx, event, "author")};

	TextRepository repo;
	TextDTO data{title, description, url, author};

	if (repo.create(data)) {
		auto msg{dpp::message(std::format(
			"E-Zine was published!\nTitle: {}\nDescription: {}\nURL: {}\nView here: https://erarnitox.de/pub",
			data.title,
			data.description,
			data.url))};

		dpp::command_completion_event_t callback = [event](const dpp::confirmation_callback_t& res) {
			const auto& message{res.get<dpp::message>()};
			Bot::ctx->message_pin(message.channel_id, message.id);
			return;
		};

		event.reply(msg);
		event.get_original_response(callback);
	} else {
		event.reply(dpp::message("Error: Document can't be saved to the database!").set_flags(dpp::m_ephemeral));
	}

	return;
}