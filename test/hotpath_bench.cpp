/*
 * Synthetic hot-path bench for message-create kernels (phrase scan + portal
 * channel-id set lookup). No Discord/Postgres. Build: hotpath_bench target.
 */

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "CppLearningIntent.hpp"

namespace {

[[nodiscard]] std::uint64_t now_ns() {
	return static_cast<std::uint64_t>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch())
			.count());
}

}  // namespace

int main() {
	constexpr int k_iters = 200000;
	const std::vector<std::string> corpus{
		"hello world",
		"how do i learn c++",
		"random chatter about rust",
		"learn cpp please",
		"short",
		"I want a tutorial for c++ this weekend",
	};

	volatile int hits = 0;
	const auto phrase_t0 = now_ns();
	for (int i = 0; i < k_iters; ++i) {
		hits += cpp_learning_intent::mentions_cpp_learning_intent(corpus[static_cast<std::size_t>(i) % corpus.size()])
					? 1
					: 0;
	}
	const auto phrase_ns = now_ns() - phrase_t0;

	std::unordered_set<std::uint64_t> portal_channels{111, 222, 333, 444, 555};
	volatile int portal_hits = 0;
	const auto portal_t0 = now_ns();
	for (int i = 0; i < k_iters * 10; ++i) {
		const std::uint64_t channel = static_cast<std::uint64_t>(i % 10000);
		portal_hits += portal_channels.contains(channel) ? 1 : 0;
	}
	const auto portal_ns = now_ns() - portal_t0;

	std::printf("phrase_scan iters=%d hits=%d ns=%llu ns_per_iter=%.2f\n",
				k_iters,
				hits,
				static_cast<unsigned long long>(phrase_ns),
				static_cast<double>(phrase_ns) / k_iters);
	std::printf("portal_set_lookup iters=%d hits=%d ns=%llu ns_per_iter=%.2f\n",
				k_iters * 10,
				portal_hits,
				static_cast<unsigned long long>(portal_ns),
				static_cast<double>(portal_ns) / (k_iters * 10));
	return 0;
}
