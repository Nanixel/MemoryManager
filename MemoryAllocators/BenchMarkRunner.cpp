#include "stdafx.h"

#include <iostream>
#include <cassert>

#include "BenchMarkRunner.h"


void BenchMarkRunner::SingleAllocation(BaseAllocator* allocator, const size_t size, const size_t alignment) {
	
	_start = _clock.now();
	unsigned int operations = 0;

	while (operations < _operationsCount) {
		allocator->allocate(size, alignment);
		operations++;
	}
	
	_end = _clock.now();

	BenchmarkResults results = buildResults(_operationsCount, calculateElapsedTime(), allocator->getUsedMemory());
	printResults(results, size);
}

//Cannot use a linear allocator here
void BenchMarkRunner::SingleFree(BaseAllocator* allocator, const size_t size, const size_t alignment) {
	constexpr int test = 300;

	void* addresses[test];

	_start = _clock.now();

	int operations = 0;
	while (operations < test) {
		addresses[operations] = allocator->allocate(size, alignment);
		operations++;
	}

	operations--;

	while (operations >= 0) {
		allocator->deallocate(addresses[operations]);
		operations--;
	}

	_end = _clock.now();

	BenchmarkResults results = buildResults(test, calculateElapsedTime(), allocator->getUsedMemory());
	printResults(results, size);
}

void BenchMarkRunner::MultipleAllocation(BaseAllocator* allocator, const std::vector<size_t>& allocationSizes,
	const std::vector<size_t>& alignments) {
	assert(allocationSizes.size() == alignments.size());

	for (int i = 0; i < allocationSizes.size(); i++) {
		SingleAllocation(allocator, allocationSizes[i], alignments[i]);
	}
}

void BenchMarkRunner::MultipleFree(BaseAllocator* allocator, const std::vector<size_t>& allocationSizes,
	const std::vector<size_t>& alignments) {
	assert(allocationSizes.size() == alignments.size());

	for (int i = 0; i < allocationSizes.size(); i++) {
		SingleFree(allocator, allocationSizes[i], alignments[i]);
	}
}

void BenchMarkRunner::printResults(const BenchmarkResults& results, const std::size_t size) const {
	//std::cout << "\tRESULTS:" << std::endl;
	std::cout << "\t\tOperations:    \t" << results.nOperations << std::endl;
	std::cout << "\t\tTime elapsed:  \t" << results.elapsedTime << " ms" << std::endl;
	//std::cout << "\t\tOp per sec:    \t" << results.operationsPerSec << " ops/ms" << std::endl;
	//std::cout << "\t\tTimer per op:  \t" << results.timePerOperation << " ms/ops" << std::endl;
	std::cout << "\t\tMemory peak:   \t" << results.memoryPeak << " bytes" << std::endl;
	std::cout << "\t\tSize:    \t" << size << std::endl;
	std::cout << std::endl;
}


const long long BenchMarkRunner::calculateElapsedTime() const {
	using std::chrono::duration_cast;
	
	using Cycle = std::chrono::duration<double, std::chrono::high_resolution_clock::period>;
	const int N = 10000;
	auto ticksPerElapsedBlock = Cycle(_end-_start) / N;
	return duration_cast<std::chrono::nanoseconds>(ticksPerElapsedBlock).count();
}


const BenchmarkResults BenchMarkRunner::buildResults(const unsigned int nOperations, const double elapsedTime,
	const std::size_t memoryUsed) const {
	BenchmarkResults results;

	results.nOperations = nOperations;
	results.elapsedTime = elapsedTime;
	results.operationsPerSec = nOperations / elapsedTime;
	results.timePerOperation = elapsedTime / nOperations;
	results.memoryPeak = memoryUsed;
	return results;
}