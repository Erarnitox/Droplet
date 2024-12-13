#include "ResourcesCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <Core.hpp>
#include <ResourceRepository.hpp>

ResourcesCommand::ResourcesCommand() : IGlobalSlashCommand() {
	this->command_name = "add_resource";
	this->command_description = "Add resource to droplet.erarnitox.de/resources";

	this->command_options.emplace_back(dpp::co_string, "title", "The title of the resource", true);

	this->command_options.emplace_back(dpp::co_string, "category", "The category of the resource", true);

	this->command_options.emplace_back(dpp::co_string, "description", "Resource description", true);

	this->command_options.emplace_back(dpp::co_string, "url", "Link to the resource", true);

	this->command_options.emplace_back(
		dpp::command_option(dpp::co_string, "difficulty", "How advanced is the resource?", true)
			.add_choice(dpp::command_option_choice("First Steps", std::string("1")))
			.add_choice(dpp::command_option_choice("Beginner", std::string("2")))
			.add_choice(dpp::command_option_choice("Intermediate", std::string("3")))
			.add_choice(dpp::command_option_choice("Advanced", std::string("4")))
			.add_choice(dpp::command_option_choice("Expert", std::string("5")))

	);

	this->command_options.emplace_back(dpp::co_string, "tags", "Link to the resource", false);
}

void ResourcesCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	const auto& cmd{event.command};

	const auto title{Core::get_parameter(*Bot::ctx, event, "title")};
	const auto category{Core::get_parameter(*Bot::ctx, event, "category")};
	const auto description{Core::get_parameter(*Bot::ctx, event, "description")};
	const auto url{Core::get_parameter(*Bot::ctx, event, "url")};
	const auto difficulty{std::stoi(Core::get_parameter(*Bot::ctx, event, "difficulty"))};
	const auto tags{Core::get_parameter(*Bot::ctx, event, "tags", false)};

	ResourceRepository repo;
	ResourceDTO data{title,
					 category,
					 description,
					 url,
					 difficulty,
					 cmd.guild_id,
					 cmd.member.get_user()->username,
					 cmd.member.user_id,
					 tags};

	if (repo.create(data)) {
		auto msg{
			dpp::message(std::format("Resource was added!\nTitle: {}\nDescription: {}\nURL: {}\nTags: {}\nView here: "
									 "https://droplet.erarnitox.de/resources",
									 data.title,
									 data.description,
									 data.url,
									 tags.empty() ? "NONE" : tags))};

		dpp::command_completion_event_t callback = [event](const dpp::confirmation_callback_t& res) {
			const auto& message{res.get<dpp::message>()};
			Bot::ctx->message_pin(message.channel_id, message.id);
			return;
		};

		event.reply(msg);
		event.get_original_response(callback);
	} else {
		event.reply(dpp::message("Error: Resource can't be saved to the database!").set_flags(dpp::m_ephemeral));
	}

	return;
}