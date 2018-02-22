#pragma once

#include <time.h>
#include <cstddef>
#include <vector>
#include <chrono>
#include "BaseAllocator.h"

struct BenchmarkResults
{
	long nOperations;
	double elapsedTime;
	float operationsPerSec;
	float timePerOperation;
	int memoryPeak;
};

class BenchMarkRunner {
private:	
	void printResults(const BenchmarkResults& results, const size_t size) const;
	void setTimer(std::chrono::high_resolution_clock& timer);

	const long long calculateElapsedTime() const;
	const BenchmarkResults buildResults(const unsigned int nOperations, const double elapsedTime, 
		const std::size_t memoryUsed) const;
private:
	using picoseconds = std::chrono::duration<unsigned long long, std::pico>;
	unsigned int _operationsCount;
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> _start, _end;
	std::chrono::high_resolution_clock _clock;

public:
	BenchMarkRunner(const unsigned int operations) : _operationsCount(operations) {};

	void SingleAllocation(BaseAllocator* allocator, const size_t size, const size_t alignment);
	void SingleFree(BaseAllocator* allocator, const size_t size, const size_t alignment);

	void MultipleAllocation(BaseAllocator* allocator, const std::vector<size_t>& allocationSizes,
		const std::vector<size_t>& alignments);
	void MultipleFree(BaseAllocator* allocator, const std::vector<size_t>& allocationSizes,
		const std::vector<size_t>& alignments);

};