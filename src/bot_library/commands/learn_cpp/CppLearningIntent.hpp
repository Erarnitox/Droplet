/*
 *  (c) Copyright erarnitox.de - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *
 *  License: MIT License
 *
 *  Description: Compile-time phrase table and ASCII lowercase scan for the
 *  "how to learn C++" message hint. Kept header-only so unit tests and the
 *  hot-path bench can measure it without Discord/DPP.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <string>
#include <string_view>

namespace cpp_learning_intent {

inline constexpr std::array<std::string_view, 28> k_phrases{
	"how to learn c++",
	"how do i learn c++",
	"how can i learn c++",
	"where to learn c++",
	"where can i learn c++",
	"want to learn c++",
	"wanna learn c++",
	"learn c++",
	"learning c++",
	"c++ tutorial",
	"tutorial for c++",
	"tutorials for c++",
	"cpp tutorial",
	"tutorial cpp",
	"learn cpp",
	"learning cpp",
	"how to learn cpp",
	"how do i learn cpp",
	"start learning c++",
	"getting started with c++",
	"beginner c++",
	"new to c++",
	"teach me c++",
	"best way to learn c++",
	"good resource c++",
	"resources to learn c++",
	"any tutorial c++",
	"recommend c++ course",
};

[[nodiscard]] inline std::string lowercase_ascii(std::string_view text) {
	std::string out(text.begin(), text.end());
	std::transform(
		out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return out;
}

[[nodiscard]] inline bool mentions_cpp_learning_intent(std::string_view original_text) {
	const std::string lower = lowercase_ascii(original_text);
	for (const std::string_view phrase : k_phrases) {
		if (lower.find(phrase) != std::string::npos) {
			return true;
		}
	}
	return false;
}

}  // namespace cpp_learning_intent
