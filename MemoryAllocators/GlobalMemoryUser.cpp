#include "stdafx.h"
#include "./GlobalMemoryUser.h"


namespace Memory {
	GlobalMemoryUser::GlobalMemoryUser() : ECS_MEMORY_MANAGER(Memory::ECSMemoryManager) {

	}

	GlobalMemoryUser::~GlobalMemoryUser() {		
	}

	void* GlobalMemoryUser::Allocate(const char* owner, size_t size) {
		return ECS_MEMORY_MANAGER->Allocate(owner, size);
	}

	void GlobalMemoryUser::Deallocate(void* startAddress) {
		ECS_MEMORY_MANAGER->Deallocate(startAddress);
	}
}

