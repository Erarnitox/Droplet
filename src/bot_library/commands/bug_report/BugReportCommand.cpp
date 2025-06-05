#include "BugReportCommand.hpp"

#include <message.h>

#include <format>

constexpr auto MODAL_ID{"bug_report_modal"};
constexpr auto FIELD_DESC{"field_description"};
constexpr auto FIELD_STEPS{"field_steps"};
constexpr auto FIELD_EXPECTED{"field_expecte"};
constexpr dpp::snowflake ERARNITOX_ID{461930808479842304};

//----------------------------------------
//
//----------------------------------------
BugReportCommand::BugReportCommand() : IGlobalSlashCommand(), IFormCommand() {
	this->command_name = "report_bug";
	this->command_description = "Report a bug to the developers";
}

//----------------------------------------
//
//----------------------------------------
void BugReportCommand::on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() != this->command_name)
		return;

	dpp::interaction_modal_response modal(MODAL_ID, "Droplet - Bug Report");

	modal.add_component(dpp::component()
							.set_type(dpp::cot_text)
							.set_label("Short description")
							.set_id(FIELD_DESC)
							.set_placeholder("What is the bug?")
							.set_text_style(dpp::text_short)
							.set_required(true));

	modal.add_row();

	modal.add_component(dpp::component()
							.set_type(dpp::cot_text)
							.set_label("Steps to Reproduce")
							.set_id(FIELD_STEPS)
							.set_placeholder("How to trigger the bug?")
							.set_text_style(dpp::text_paragraph)
							.set_required(true));

	modal.add_row();

	modal.add_component(dpp::component()
							.set_type(dpp::cot_text)
							.set_label("Expected Behavior")
							.set_id(FIELD_EXPECTED)
							.set_placeholder("What should have happened?")
							.set_text_style(dpp::text_paragraph)
							.set_required(true));

	event.dialog(modal);
}

//----------------------------------------
//
//----------------------------------------
void BugReportCommand::on_form_submit(const dpp::form_submit_t& event) {
	if (event.custom_id != MODAL_ID)
		return;

	const auto description{std::get<std::string>(event.components.at(0).components.at(0).value)};
	const auto steps{std::get<std::string>(event.components.at(1).components.at(0).value)};
	const auto expected{std::get<std::string>(event.components.at(2).components.at(0).value)};

	const auto message{
		std::format("**Bug Report by {}**\n\n"
					"**Description:**\n{}\n\n"
					"**Steps to Reproduce:**\n{}\n\n"
					"**Expected Behavior:**\n{}",
					event.command.get_issuing_user().format_username(),
					description,
					steps,
					expected)};

	Bot::ctx->direct_message_create(
		ERARNITOX_ID, dpp::message(message), [event](const dpp::confirmation_callback_t& cc) {
			if (not cc.is_error()) {
				Core::timed_reply_private(*Bot::ctx, event, "Thanks for your report!", 3000);
			} else {
				Core::timed_reply_private(
					*Bot::ctx, event, "Sending failed! Please contact @erarnitox directly!", 5000);
			}
		});
}
