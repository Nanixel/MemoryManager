#pragma once
#include <chrono>

namespace Utility {
	namespace Time {
		const long long CalculateElapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> _start,
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> _end) {
			using std::chrono::duration_cast;

			using Cycle = std::chrono::duration<double, std::chrono::high_resolution_clock::period>;
			const int N = 10000;
			auto ticksPerElapsedBlock = Cycle(_end - _start) / N;
			return duration_cast<std::chrono::nanoseconds>(ticksPerElapsedBlock).count();
		}
	}
}