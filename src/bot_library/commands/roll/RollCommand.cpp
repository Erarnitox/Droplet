#include "RollCommand.hpp"
#include "UserRepository.hpp"
#include "UserDTO.hpp"

#include <appcommand.h>
#include <invite.h>
#include <message.h>

RollCommand::RollCommand() : IGlobalSlashCommand() {
	this->command_name = "roll";
	this->command_description = "Roll a dice";
	this->command_options.emplace_back(dpp::command_option(dpp::co_integer, "bidding", "Bidding amount in EXP", true));
}

void RollCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name) {
		return;
	}

	long bidding{ std::get<long>(event.get_parameter("bidding")) };
	if(bidding < 1) {
		event.reply(dpp::message("Bidding amount can't be smaller than 1").set_flags(dpp::m_ephemeral));
		return;
	}

	// Get the user sending the event
	const auto& member { event.command.member };
	if (!member.user_id) {
		event.reply(dpp::message("Can't get the member using this command. Try again!").set_flags(dpp::m_ephemeral));
		return;
	}

	// Get the user from the database
	UserRepository user_repo;
	UserDTO user_dto{};

	try {
		user_dto = user_repo.get(static_cast<size_t>(member.user_id));
	} catch (...) {
		Bot::ctx->log(dpp::ll_warning, "User is not in usr database yet");
	}

	if(static_cast<size_t>(bidding) > 9'999'999ull){
		event.reply(dpp::message("Careful High Roller! Your Bidding is too damn high!").set_flags(dpp::m_ephemeral));
		return;
	}

	if(user_dto.exp < static_cast<size_t>(bidding)) {
		event.reply(dpp::message("Your EXP-Balance is too low!").set_flags(dpp::m_ephemeral));
		return;
	}

	static std::string number_urls[]{std::string("https://erarnitox.de/res/dice/1.png"),
									 std::string("https://erarnitox.de/res/dice/2.png"),
									 std::string("https://erarnitox.de/res/dice/3.png"),
									 std::string("https://erarnitox.de/res/dice/4.png"),
									 std::string("https://erarnitox.de/res/dice/5.png"),
									 std::string("https://erarnitox.de/res/dice/6.png")};

	const auto result{std::rand() % 6};

	if(result > 3){
		const auto multiplier{ result - 3};
		user_dto.exp += static_cast<size_t>(bidding * multiplier);
	} else {
		user_dto.exp -= static_cast<size_t>(bidding);
	}

	if(!user_repo.update(user_dto)){
		event.reply(dpp::message("Oh no! Something went wrong! Sowwy! :c").set_flags(dpp::m_ephemeral));
		return;
	}

	/* create the embed */
	dpp::embed embed{dpp::embed()
		.set_color((result > 3) ? dpp::colors::green : dpp::colors::red)
		.set_title((result > 3) ? std::format("WON! - {}", (result+1)) : std::format("LOST - {}", (result+1)))
		.set_image(number_urls[result])
		.add_field("New Balance", std::format("{}EXP", user_dto.exp))
	};

	/* reply with the created embed */
	event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));

	return;
}