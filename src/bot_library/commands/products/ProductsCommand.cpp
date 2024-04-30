#include "ProductsCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>
#include <message.h>

#include <Core.hpp>
#include <ProductRepository.hpp>

ProductsCommand::ProductsCommand() : IGlobalSlashCommand() {
	this->command_name = "add_product";

	this->command_description = "Add a product to dropsoft.org (Dropsoft only!)";

	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "name", "Name of the Product", true));

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "picture_url", "Preview Picture URL", true));

	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "button_url", "Link to the Product", true));
	this->command_options.emplace_back(dpp::command_option(dpp::co_string, "button_text", "Message on the Button", true));
}

void ProductsCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	if (!Core::is_dropsoft_admin(event.command.member)) {
		event.reply("Only Dropsoft-Admins are allowed to run this command!");
		return;
	}

	const auto name{Core::get_parameter(*Bot::ctx, event, "name")};
	const auto picture_url{Core::get_parameter(*Bot::ctx, event, "picture_url")};
	const auto button_url{Core::get_parameter(*Bot::ctx, event, "button_url")};
	const auto button_text{Core::get_parameter(*Bot::ctx, event, "button_text")};
	

	ProductRepository repo;
	ProductDTO data{ 
		static_cast<size_t>(random()%10000), 
		picture_url, button_text,
		0, name, button_url, 
		"", 
		ProductDTO::Type::DOWNLOAD 
	};
	
	if (repo.create(data)) {
		auto msg{dpp::message("Product was added!").set_flags(dpp::m_ephemeral)};

		dpp::command_completion_event_t callback = [event](const dpp::confirmation_callback_t& res) {
			const auto& message{res.get<dpp::message>()};
			Bot::ctx->message_pin(message.channel_id, message.id);
			return;
		};

		event.reply(msg);
		event.get_original_response(callback);
	} else {
		event.reply(dpp::message("Error: Product can't be saved to the database!").set_flags(dpp::m_ephemeral));
	}
	
	return;
}