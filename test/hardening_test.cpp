#include <unistd.h>

#include <AuthUtils.hpp>
#include <ConnectionPool.hpp>
#include <Supervisor.hpp>
#include <Watchdog.hpp>
#include <YoutubeFeed.hpp>
#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>

namespace {

// Each (forked) worker run appends one line to this file, so the parent can count runs.
struct RunCounter {
	std::filesystem::path path;

	RunCounter()
		: path{std::filesystem::temp_directory_path() / ("droplet_supervisor_test_" + std::to_string(::getpid()))} {
		std::filesystem::remove(path);
	}
	~RunCounter() {
		std::filesystem::remove(path);
	}

	[[nodiscard]] int next() const {  // number of previous runs; records this one
		const int previous{count()};
		std::ofstream out{path, std::ios::app};
		out << "run\n";
		return previous;
	}

	[[nodiscard]] int count() const {
		std::ifstream in{path};
		int lines{0};
		for (std::string line; std::getline(in, line);) {
			++lines;
		}
		return lines;
	}
};

supervisor::Options fast_options() {
	supervisor::Options options;
	options.min_backoff = std::chrono::milliseconds{10};
	options.max_backoff = std::chrono::milliseconds{50};
	return options;
}

}  // namespace

TEST_CASE("supervisor returns when the worker exits cleanly", "[supervisor]") {
	const RunCounter counter;
	const int rc{supervisor::run(
		[&] {
			(void)counter.next();
			return 0;
		},
		fast_options())};
	CHECK(rc == 0);
	CHECK(counter.count() == 1);
}

TEST_CASE("supervisor restarts a worker that exits with an error", "[supervisor]") {
	const RunCounter counter;
	const int rc{supervisor::run([&] { return counter.next() < 2 ? 3 : 0; }, fast_options())};
	CHECK(rc == 0);
	CHECK(counter.count() == 3);
}

TEST_CASE("supervisor restarts a worker that is killed by a signal", "[supervisor]") {
	const RunCounter counter;
	const int rc{supervisor::run(
		[&] {
			if (counter.next() < 2) {
				std::raise(SIGKILL);  // simulates a segfault / OOM kill
			}
			return 0;
		},
		fast_options())};
	CHECK(rc == 0);
	CHECK(counter.count() == 3);
}

TEST_CASE("supervisor restarts a worker that throws", "[supervisor]") {
	const RunCounter counter;
	const int rc{supervisor::run(
		[&]() -> int {
			if (counter.next() < 1) {
				throw std::runtime_error("boom");
			}
			return 0;
		},
		fast_options())};
	CHECK(rc == 0);
	CHECK(counter.count() == 2);
}

TEST_CASE("watchdog fires when it is not petted", "[watchdog]") {
	std::atomic<bool> fired{false};
	Watchdog watchdog{std::chrono::seconds{1}, [&](std::chrono::seconds) { fired = true; }};
	watchdog.start();
	for (int i{0}; i < 50 && not fired.load(); ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds{100});
	}
	CHECK(fired.load());
}

TEST_CASE("watchdog stays quiet while it is petted", "[watchdog]") {
	std::atomic<bool> fired{false};
	Watchdog watchdog{std::chrono::seconds{2}, [&](std::chrono::seconds) { fired = true; }};
	watchdog.start();
	for (int i{0}; i < 30; ++i) {
		watchdog.pet();
		std::this_thread::sleep_for(std::chrono::milliseconds{100});
	}
	watchdog.stop();
	CHECK_FALSE(fired.load());
}

TEST_CASE("with_connection_defaults adds timeouts and keepalives", "[database]") {
	const auto keyvalue{with_connection_defaults("host=db dbname=bot user=u password=p")};
	CHECK(keyvalue.starts_with("host=db dbname=bot user=u password=p "));
	CHECK(keyvalue.find("connect_timeout=10") != std::string::npos);
	CHECK(keyvalue.find("keepalives=1") != std::string::npos);
	CHECK(keyvalue.find("tcp_user_timeout=30000") != std::string::npos);

	const auto uri{with_connection_defaults("postgresql://u:p@db/bot")};
	CHECK(uri.starts_with("postgresql://u:p@db/bot?connect_timeout=10&"));
	CHECK(uri.find("?") == uri.rfind("?"));
}

TEST_CASE("with_connection_defaults respects explicit settings", "[database]") {
	const auto result{with_connection_defaults("host=db connect_timeout=3 keepalives=0")};
	CHECK(result.find("connect_timeout=3") != std::string::npos);
	CHECK(result.find("connect_timeout=10") == std::string::npos);
	CHECK(result.find("keepalives=0") != std::string::npos);
	CHECK(result.find("keepalives=1") == std::string::npos);
}

TEST_CASE("ConnectionPool reports failure for an unreachable database without hanging", "[database]") {
	ConnectionPool pool;
	CHECK_FALSE(pool.connect("host=127.0.0.1 port=1 dbname=none connect_timeout=2"));
	CHECK_FALSE(pool.has_connection());
	auto lease{pool.acquire()};
	CHECK_FALSE(static_cast<bool>(lease));
}

TEST_CASE("rate limiter memory is bounded", "[auth]") {
	RequestRateLimiter limiter{5, std::chrono::seconds{60}};
	bool refused{false};
	for (std::size_t i{0}; i < k_max_tracked_keys + 10; ++i) {
		if (not limiter.allow("client-" + std::to_string(i))) {
			refused = true;
			break;
		}
	}
	CHECK(refused);
	// clients that are already tracked keep working
	CHECK(limiter.allow("client-0"));
}

TEST_CASE("parse_youtube_feed survives hostile input", "[youtube]") {
	CHECK(parse_youtube_feed("<entry>").empty());
	CHECK(parse_youtube_feed("<entry><yt:videoId>abc</yt:videoId>").empty());  // unterminated entry

	const auto with_attributes{
		parse_youtube_feed("<entry><yt:videoId>abc</yt:videoId><title type=\"text\">Hi &amp; bye</title></entry>")};
	REQUIRE(with_attributes.size() == 1);
	CHECK(with_attributes[0].title == "Hi & bye");

	// libstdc++'s std::regex would recurse once per character here
	const std::string huge{"<entry><yt:videoId>big</yt:videoId><title>" + std::string(8'000'000, 'a') +
						   "</title></entry>"};
	const auto parsed{parse_youtube_feed(huge)};
	REQUIRE(parsed.size() == 1);
	CHECK(parsed[0].title.size() == 8'000'000);
}
