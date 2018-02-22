#pragma once

#include "./StackAllocator.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <list>

#define GLOBAL_MEMORY_CAPACITY 134217728


namespace Memory {


	class MemoryManager {

		friend class GlobalMemoryUser;

	private:

		void*										_globalMemory;
		StackAllocator*								_allocator;
		std::vector<std::pair<const char*, void*>> _pendingMemory;
		std::list<void*>						   _freeMemory;

		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(MemoryManager&) = delete;

	public:

		static constexpr size_t MEMORY_CAPACITY = GLOBAL_MEMORY_CAPACITY;
		
		MemoryManager();					
		~MemoryManager();

		void* Allocate(const char* user, size_t size) {
			
			std::cout << user << " allocated " << size << " bytes of global memory." << std::endl;
			void* memory = _allocator->allocate(size, alignof(u8));
			_pendingMemory.push_back({ user, memory });
			return memory;

		}
		void Deallocate(void* startAddress) {

			//if this address is at the top of the stack, deallocate it, else: add it to the _freeMemoryList
			if (startAddress == _pendingMemory.end()->second) {
				_allocator->deallocate(startAddress);
				_pendingMemory.pop_back();
				bool bcheck = true;

				//check if the next thing on the top of the stack is in the freeMemoryList
				while (bcheck) {
					bcheck = false;
					for (auto it : _freeMemory) {
						if (it = _pendingMemory.back().second) {
							_allocator->deallocate(it);
							_pendingMemory.pop_back();
							_freeMemory.remove(it);
							bcheck = true;
							break;
						}
					}
				}
			}
			else {
				_freeMemory.push_back(startAddress);
			}

		}

	};

}
