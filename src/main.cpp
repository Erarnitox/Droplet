#include <dpp/dpp.h>
#include <string>

const std::string BOT_TOKEN{ std::string("MTA3MTE0OTYxMjMwNTQ5ODE2Mg.GNiJSY.1EVEMLFlkOJfGFvbC5hy00fdKm7IRyXZj_mENs") };

auto main() -> int {
	dpp::cluster bot(BOT_TOKEN);

	//message listener
	bot.on_message_create([&bot](const dpp::message_create_t& event) {
			if(event.msg.content == "!info") {

				//create an embed
				dpp::embed embed = dpp::embed().
					set_color(dpp::colors::sti_blue).
					set_title("DropSoft").
					set_url("https://www.dropsoft.org").
					set_author("DropSoft", "https://dropsoft.org", 
							"https://www.dropsoft.org/img/logo.png").
					set_description("DropSoft").
					set_thumbnail("https://www.dropsoft.org/img/logo.png").
					add_field(
						"Hello there!",
						"This is a test"
					).
					add_field(
						"Inline field",
						"Some value",
						true
					).
					add_field(
						"another inline",
						"drop drop"
					).
					set_image("https://www.dropsoft.org/img/logo.png").
					set_footer(dpp::embed_footer().set_text("DropSoft all the way!").
					set_icon("https://www.dropsoft.org/img/logo.png")).
					set_timestamp(time(0));
	
				bot.message_create(dpp::message(event.msg.channel_id, embed).
					set_reference(event.msg.id));
				}

				//send files
				if(event.msg.content == "!file") {
					dpp::message msg(event.msg.channel_id, "hey there!");
					msg.add_file("test.txt", dpp::utility::read_file(""));
					bot.message_create(msg);
			}
	});

	//command listener
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
			if(event.command.get_command_name() == "drop") {
				event.reply("Just a drop of Water in an endless sea");
			}
	});

	//command registration
	bot.on_ready([&bot](const dpp::ready_t& event) {
			if(dpp::run_once<struct register_bot_commands>()) {
				bot.global_command_create(
						dpp::slashcommand("drop", "Run to see what it does!", bot.me.id)
				);
			}
	});

	bot.start(dpp::st_wait);
	return 0;
}
