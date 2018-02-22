// MemoryAllocators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "./LinearAllocator.h"
#include "./StackAllocator.h"
#include "./PoolAllocator.h"
#include "./BenchMarkRunner.h"
#include "./CAllocator.h"
#include "./Utility.h"
#include <ctime>
#include <vector>
#include "./ComponentContainer.h"

#define GLOBAL_MEMORY_CAPACITY_FOR_ALLOCATOR_TESTS 134217728
static constexpr size_t MEM_CAPCITY = GLOBAL_MEMORY_CAPACITY_FOR_ALLOCATOR_TESTS;
void* GlobalMemory;

class Person {
public:
	Person(std::string name, int age) : Name(name), age(age) {}
	std::string Name;
	int age;

	std::string test1 = "test";
	std::string test2 = "test2";
	std::string test3 = "test3";
	std::string test4 = "test4";

};
std::vector<std::size_t> ALLOCATION_SIZES{ 32, 64, 256, 1024, 2048, 4096 };
std::vector<std::size_t> ALIGNMENTS{ 8,8,8,8,8,8 };

std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> _start, _end;
std::chrono::high_resolution_clock _clock;

void DoBenchmarkingCAllocator() {	
	CAllocator* cAllocator = new CAllocator(4194304, GlobalMemory);	
	BenchMarkRunner runner(300);

	std::cout << "C\b" << std::endl;
	std::cout << "ALLOCATE: \b" << std::endl;
	runner.MultipleAllocation(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);
	std::cout << "FREE: \b" << std::endl;
	runner.MultipleFree(cAllocator, ALLOCATION_SIZES, ALIGNMENTS);

	//clear the memory
}

void DoBenchMarkingPoolAllocator() {
	std::vector<std::size_t> ALLOCATION_SIZES_POOL{ 4096 };
	std::vector<std::size_t> ALIGNMENTS_POOL{ 4 };
	PoolAllocator* poolAllocator = new PoolAllocator(4194304, GlobalMemory, 4096, __alignof(Person));
	BenchMarkRunner runner(300);

	std::cout << "POOL\b" << std::endl;
	std::cout << "ALLOCATE\b" << std::endl;
	runner.MultipleAllocation(poolAllocator, ALLOCATION_SIZES_POOL, ALIGNMENTS_POOL);
	std::cout << "FREE\b" << std::endl;			
	runner.MultipleFree(poolAllocator, ALLOCATION_SIZES_POOL, ALIGNMENTS_POOL);
}

void DoBenchmarkingStackAllocator() {
	StackAllocator* stackAllocator = new StackAllocator(4194304, GlobalMemory);
	BenchMarkRunner runner(300);

	std::cout << "STACK\b" << std::endl;
	std::cout << "ALLOCATE\b" << std::endl;
	runner.MultipleAllocation(stackAllocator, ALLOCATION_SIZES, ALIGNMENTS);
	std::cout << "FREE\b" << std::endl;
	runner.MultipleFree(stackAllocator, ALLOCATION_SIZES, ALIGNMENTS);
}

void DoBenchmarkingLinearAllocator() {
	LinearAllocator* allocator = new LinearAllocator(4194304, GlobalMemory);
	BenchMarkRunner runner(300);

	std::cout << "STACK\b" << std::endl;
	std::cout << "ALLOCATE\b" << std::endl;
	runner.MultipleAllocation(allocator, ALLOCATION_SIZES, ALIGNMENTS);
}

void ComponentContainerExample() {	
	ComponentContainer<Person>* container = new ComponentContainer<Person>();
	void* memory = container->CreateObject();
	Person* person = new(memory) Person("Test", 7);			
}

int main()
{

	//GlobalMemory = malloc(MEM_CAPCITY);

	///BenchMark Allocators
	//DoBenchmarkingCAllocator();
	//DoBenchmarkingStackAllocator();
	//DoBenchMarkingPoolAllocator();
	//DoBenchmarkingLinearAllocator();

	//ComponentContainerExample();

    return 0;
}

