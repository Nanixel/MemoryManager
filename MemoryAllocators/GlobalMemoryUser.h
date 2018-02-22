#pragma once
#include <cstdint>
#include "./MemoryManager.h"
#include "./API.h"

using u8 = uint8_t;

namespace Memory {

	class GlobalMemoryUser {
	private:
		 MemoryManager* ECS_MEMORY_MANAGER;

	public:
		GlobalMemoryUser();
		
		~GlobalMemoryUser();

		void* Allocate(const char* owner, size_t size);
		void Deallocate(void* startAddress);
	};

}
