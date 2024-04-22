#include "ProductsCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <Core.hpp>
#include <ProductRepository.hpp>

ProductsCommand::ProductsCommand() : IGlobalSlashCommand() {
	this->command_name = "add_product";
	this->command_description = "Add a pdf to dropsoft.org/papers (Admin only!)";

	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "title", "The title of the Document", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "description", "Document description", true));

	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "url", "Link to the document", true));
	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "author", "Author of the document", true));
}

void ProductsCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (event.command.guild_id != dpp::snowflake(808151108748836914ull)) {
		event.reply("This command can only be executed on the DropSoft Server!");
		return;
	}

	if (!Core::is_admin(event.command.member)) {
		event.reply("Only admins are allowed to run this command!");
		return;
	}

	const auto title{Core::get_parameter(*Bot::ctx, event, "title")};
	const auto description{Core::get_parameter(*Bot::ctx, event, "description")};
	const auto url{Core::get_parameter(*Bot::ctx, event, "url")};
	const auto author{Core::get_parameter(*Bot::ctx, event, "author")};

	ProductRepository repo;
	ProductDTO data{title, description, url, author};

	if (repo.create(data)) {
		auto msg{dpp::message(std::format(
			"Document was added!\nTitle: {}\nDescription: {}\nURL: {}\nView here: https://dropsoft.org/papers",
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