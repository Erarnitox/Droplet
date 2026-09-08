#include <catch2/catch_test_macros.hpp>

#include <YoutubeFeed.hpp>
#include <string>
#include <unordered_set>

namespace {

constexpr const char* kSampleAtom = R"(<?xml version="1.0" encoding="UTF-8"?>
<feed xmlns:yt="http://www.youtube.com/xml/schemas/2015" xmlns:media="http://search.yahoo.com/mrss/" xmlns="http://www.w3.org/2005/Atom">
 <title>Erarnitox</title>
 <published>2014-09-15T09:33:13+00:00</published>
 <entry>
  <yt:videoId>newerVid002</yt:videoId>
  <title>Second new upload</title>
  <published>2026-09-08T15:00:00+00:00</published>
  <updated>2026-09-08T15:00:10+00:00</updated>
 </entry>
 <entry>
  <yt:videoId>newerVid001</yt:videoId>
  <title>First new upload</title>
  <published>2026-09-08T14:00:00+00:00</published>
  <updated>2026-09-08T14:00:10+00:00</updated>
 </entry>
 <entry>
  <yt:videoId>ZdEfSoqkkgo</yt:videoId>
  <title>My thoughts as a C++ Developer</title>
  <published>2026-09-08T13:30:16+00:00</published>
  <updated>2026-09-08T13:30:35+00:00</updated>
 </entry>
 <entry>
  <yt:videoId>-hlUJbEWnI4</yt:videoId>
  <title>Trying To Survive The Impossible Wave</title>
  <published>2026-02-05T20:54:36+00:00</published>
  <updated>2026-09-07T21:14:08+00:00</updated>
 </entry>
 <entry>
  <yt:videoId>-oAIBPxUUEc</yt:videoId>
  <title>Lambdas &amp; Callbacks in C++</title>
  <published>2025-10-16T13:30:00+00:00</published>
  <updated>2026-07-13T15:12:04+00:00</updated>
 </entry>
</feed>
)";

}  // namespace

TEST_CASE("parse_youtube_feed reads every Atom entry", "[youtube]") {
	const auto entries = parse_youtube_feed(kSampleAtom);

	REQUIRE(entries.size() == 5);
	CHECK(entries[0].video_id == "newerVid002");
	CHECK(entries[0].published == "2026-09-08T15:00:00+00:00");
	CHECK(entries[1].video_id == "newerVid001");
	CHECK(entries[2].video_id == "ZdEfSoqkkgo");
	CHECK(entries[3].video_id == "-hlUJbEWnI4");
	CHECK(entries[4].video_id == "-oAIBPxUUEc");
	CHECK(entries[4].title == "Lambdas & Callbacks in C++");
}

TEST_CASE("parse_youtube_feed returns empty on HTML or missing entries", "[youtube]") {
	CHECK(parse_youtube_feed("").empty());
	CHECK(parse_youtube_feed("<html><body>consent</body></html>").empty());
}

TEST_CASE("plan_youtube_uploads seeds the whole window on first sight", "[youtube]") {
	const auto entries = parse_youtube_feed(kSampleAtom);
	const auto plan = plan_youtube_uploads(entries, {});

	CHECK(plan.to_announce.empty());
	REQUIRE(plan.to_seed.size() == entries.size());
	CHECK(plan.to_seed[0].video_id == "newerVid002");
}

TEST_CASE("plan_youtube_uploads does not announce older unseeded videos", "[youtube]") {
	const auto entries = parse_youtube_feed(kSampleAtom);
	const std::unordered_set<std::string> known{"ZdEfSoqkkgo"};
	const auto plan = plan_youtube_uploads(entries, known);

	REQUIRE(plan.to_announce.size() == 2);
	CHECK(plan.to_announce[0].video_id == "newerVid001");
	CHECK(plan.to_announce[1].video_id == "newerVid002");

	REQUIRE(plan.to_seed.size() == 2);
	CHECK(plan.to_seed[0].video_id == "-hlUJbEWnI4");
	CHECK(plan.to_seed[1].video_id == "-oAIBPxUUEc");
}

TEST_CASE("plan_youtube_uploads seeds the window when no announced id is in the feed", "[youtube]") {
	const auto entries = parse_youtube_feed(kSampleAtom);
	const std::unordered_set<std::string> known{"fallenOffTip"};
	const auto plan = plan_youtube_uploads(entries, known);

	CHECK(plan.to_announce.empty());
	REQUIRE(plan.to_seed.size() == entries.size());
}
