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
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "Watchdog.hpp"

namespace {

constexpr int k_exit_config{78};  // EX_CONFIG: missing / unreadable token or database credentials
constexpr int k_exit_unavailable{69};  // EX_UNAVAILABLE: Discord client stopped / failed

// The Discord library runs timers on its event-loop thread. If that thread stops
// ticking for this long the bot is hung, so the watchdog restarts the process.
constexpr std::chrono::seconds k_watchdog_timeout{300};
constexpr std::uint64_t k_heartbeat_interval_sec{15};

[[noreturn]] void exit_now(int code) {
	std::fflush(nullptr);
	std::_Exit(code);
}

// sleeps `total`, waking early when `stop` becomes true
void sleep_unless_stopped(const std::atomic<bool>& stop, std::chrono::seconds total) {
	for (auto left{total}; left.count() > 0 && not stop.load(); left -= std::chrono::seconds{1}) {
		std::this_thread::sleep_for(std::chrono::seconds{1});
	}
}

// The database can be down when the bot (re)starts, e.g. after a host reboot.
// Waiting for it is better than exiting: the portal/blacklist indices below are
// built from it once, and an empty index would silently disable features.
[[nodiscard]] bool connect_database(ConnectionPool& pool, const std::string& connection_string, bool single_attempt) {
	std::chrono::seconds delay{1};
	for (int attempt{1};; ++attempt) {
		if (pool.connect(connection_string)) {
			return true;
		}
		if (single_attempt) {
			std::cerr << "Database connection failed\n";
			return false;
		}
		std::cerr << "Database connection failed (attempt " << attempt << "), retrying in " << delay.count() << "s\n";
		std::this_thread::sleep_for(delay);
		delay = std::min(delay * 2, std::chrono::seconds{30});
	}
}

void run_rest_server(DatabaseExecutor& db, const std::atomic<bool>& stopping) {
	std::chrono::seconds delay{5};
	while (not stopping.load()) {
		try {
			std::cout << "Starting REST Server..." << std::endl;
			const int code{RestApi::start(db)};
			if (code == 0) {
				return;	 // termination was requested
			}
			std::cerr << "REST server stopped with exit code " << code << '\n';
		} catch (const std::exception& ex) {
			std::cerr << "REST server failed: " << ex.what() << '\n';
		} catch (...) {
			std::cerr << "REST server failed with a non-standard exception\n";
		}
		std::cerr << "Restarting REST server in " << delay.count() << "s\n";
		sleep_unless_stopped(stopping, delay);
		delay = std::min(delay * 2, std::chrono::seconds{300});
	}
}

}  // namespace

int Application::run(bool is_test) {
	std::unique_ptr<Bot> bot;
	std::string db_connection_string;
	try {
		bot = std::make_unique<Bot>(read_bot_token("bot_token.txt"));
		db_connection_string = read_database_credentials("db_connection.txt");
	} catch (const std::exception& ex) {
		std::cerr << ex.what() << '\n';
		return k_exit_config;
	}

	ConnectionPool pool;
	if (not connect_database(pool, db_connection_string, is_test)) {
		bot->shutdown();
		return EXIT_FAILURE;
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
		return EXIT_SUCCESS;
	}

	std::atomic<bool> stopping{false};
	std::thread rest_thread([&db, &stopping] { run_rest_server(db, stopping); });

	// heartbeat: a timer ticks on the Discord event-loop thread; the watchdog
	// terminates the process if that stops happening (deadlock / blocked loop)
	Watchdog watchdog{k_watchdog_timeout};
	(void)bot->discord().start_timer([&watchdog](dpp::timer) { watchdog.pet(); }, k_heartbeat_interval_sec);
	watchdog.start();

	std::cout << "Starting Discord Bot..." << std::endl;
	try {
		bot->run();
	} catch (const std::exception& ex) {
		std::cerr << "Discord client failed: " << ex.what() << '\n';
	} catch (...) {
		std::cerr << "Discord client failed with a non-standard exception\n";
	}

	// bot->run() only returns if the Discord client stopped (or failed to start,
	// e.g. no network at boot). The REST thread would wait forever, so do not join:
	// exit with a failure code and let the supervisor / service manager restart us.
	std::cerr << "Discord client stopped, exiting\n";
	stopping = true;
	youtube.stop_all();
	exit_now(k_exit_unavailable);
}
