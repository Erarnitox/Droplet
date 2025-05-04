#include "GetResourcesCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <Core.hpp>
#include <ResourceRepository.hpp>

GetResourcesCommand::GetResourcesCommand() : IGlobalSlashCommand() {
	this->command_name = "get_resource";
	this->command_description = "Query resources from droplet.erarnitox.de/resources";
	this->command_options.emplace_back(dpp::co_string, "category", "The category of the resource", true);
}

void GetResourcesCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	const auto category{Core::get_parameter(*Bot::ctx, event, "category", true)};

	ResourceRepository repo;

	const std::vector<ResourceDTO> resources = repo.get(category);

	/* create the embed */
	constexpr auto icon_url{"https://www.erarnitox.de/favicon-32x32.png"};
	dpp::embed embed{dpp::embed()
						 .set_color(dpp::colors::discord_black)
						 .set_title("RESOURCES:")
						 .set_url("https://droplet.erarnitox.de/resources")
						 .set_description(std::format("Category: [{}]", category))
						 .set_thumbnail(icon_url)};

	for (auto& res : resources) {
		embed.add_field(res.title, res.url);
	}

	auto footer{dpp::embed_footer()};
	footer.set_icon(icon_url);
	footer.set_text("Visit https://droplet.erarnitox.de/resources");
	embed.set_footer(footer);

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	return;
}