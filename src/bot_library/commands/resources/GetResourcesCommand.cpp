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

#include "GetResourcesCommand.hpp"

#include <appcommand.h>
#include <colors.h>
#include <dispatcher.h>

#include <AppContext.hpp>
#include <Core.hpp>
#include <ResourceRepository.hpp>

//-----------------------------------------------------
//
//-----------------------------------------------------
GetResourcesCommand::GetResourcesCommand(AppContext& ctx) : discord_(ctx.discord), db_(ctx.db) {
	this->command_name = std::string(k_name);
	this->command_description = std::string(k_description);
	this->command_options.emplace_back(dpp::co_string, "category", "The category of the resource", true);
}

//-----------------------------------------------------
//
//-----------------------------------------------------
void GetResourcesCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	const auto category{Core::get_parameter(discord_, event, "category", true)};

	ResourceRepository repo{db_};

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