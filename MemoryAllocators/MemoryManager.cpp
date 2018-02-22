#include "stdafx.h"
#include "./MemoryManager.h"

namespace Memory {

	MemoryManager::MemoryManager() {
		_globalMemory = malloc(GLOBAL_MEMORY_CAPACITY);
		_allocator = new StackAllocator(GLOBAL_MEMORY_CAPACITY, _globalMemory);
		_pendingMemory.clear();
		_freeMemory.clear();
	}

	MemoryManager::~MemoryManager() {
		_allocator->clear();
		delete _allocator;
		_allocator = nullptr;

		Deallocate(_globalMemory);
		_globalMemory = nullptr;		
	}

}