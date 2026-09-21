/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 */

#include "Application.hpp"

#include <AppContext.hpp>
#include <BlacklistRepository.hpp>
#include <Bot.hpp>
#include <BotLibrary.hpp>
#include <Commands.hpp>
#include <ConnectionPool.hpp>
#include <DatabaseExecutor.hpp>
#include <PortalIndex.hpp>
#include <PortalRepository.hpp>
#include <RestApi.hpp>
#include <YoutubeNotificationService.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

void Application::run(bool is_test) {
	std::unique_ptr<Bot> bot;
	try {
		bot = std::make_unique<Bot>(read_bot_token("bot_token.txt"));
	} catch (const std::exception& ex) {
		std::cerr << ex.what();
		return;
	}

	ConnectionPool pool;
	try {
		const auto db_connection_string{read_database_credentials("db_connection.txt")};
		if (not pool.connect(db_connection_string)) {
			bot->shutdown();
			return;
		}
	} catch (const std::exception& ex) {
		std::cerr << ex.what();
		bot->shutdown();
		return;
	}

	DatabaseExecutor db{pool};
	PortalIndex portal_index;
	BlacklistIndex blacklist_index;
	YoutubeNotificationService youtube{bot->discord(), db};

	{
		PortalRepository portals{db};
		portal_index.rebuild(portals.getAll());
		BlacklistRepository blacklist{db};
		std::vector<std::string> names;
		for (const auto& row : blacklist.getAll()) {
			names.push_back(row.username);
		}
		blacklist_index.rebuild(names);
	}

	AppContext ctx{bot->discord(), db, portal_index, blacklist_index, youtube, bot->slash_commands};

	Commands::registerCommands(*bot, ctx);

	if (is_test) {
		bot->shutdown();
		pool.disconnect();
		return;
	}

	std::cout << "Starting REST Server..." << std::endl;
	std::thread rest_thread([&db] { RestApi::start(db); });

	std::cout << "Starting Discord Bot..." << std::endl;
	bot->run();

	std::cout << "Shutting down..." << std::endl;
	youtube.stop_all();
	rest_thread.join();
	bot->shutdown();
	pool.disconnect();
}
