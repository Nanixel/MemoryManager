#pragma once
#include <cassert>
#include <iostream>
#include <cstdint>

using u8 = uint8_t;

namespace pointer_math {
	static inline void* AlignForward(void* address, u8 alignment) {
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	static inline u8 GetAdjustmentForAddress(const void* address, u8 alignment) {
		u8 adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
		return adjustment == alignment ? 0 : adjustment;
	}

	//Some allocators need to store an header before each allocation so they can use the adjustment 
	//space to reduce the memory overhead caused by the headers.
	static inline u8 GetAdjustmentForAddress(const void* address, u8 alignment, u8 extra)
	{
		u8 adjustment = GetAdjustmentForAddress(address, alignment);

		u8 neededSpace = extra;

		if (adjustment < neededSpace) {
			neededSpace -= adjustment;

			//Increase adjustment to fit to header
			adjustment += adjustment * (neededSpace / alignment);

			if (neededSpace % alignment > 0) {
				adjustment += alignment;
			}
		}

		return alignment;
	}

	inline void *add(void *p, size_t x) {

		return (void*)(reinterpret_cast<uintptr_t>(p) + x);
	}

	inline const void *add(const void *p, size_t x) {

		return (const void*)(reinterpret_cast<uintptr_t>(p) + x);
	}


	inline void *subtract(void *p, size_t x) {

		return (void*)(reinterpret_cast<uintptr_t>(p) - x);
	}

	inline const void *subtract(const void *p, size_t x) {

		return (const void*)(reinterpret_cast<uintptr_t>(p) - x);
	}

}


class BaseAllocator {
protected:
	const void* _start;
	size_t _size;
	size_t _usedMemory;
	size_t _allocationsCount;

public:
	BaseAllocator(size_t size, void* start) : _size(size), _start(start), _usedMemory(0), _allocationsCount(0) {}

	virtual ~BaseAllocator() {
		if (_allocationsCount == 0 && _usedMemory == 0) {
			std::cout << "Memory Leak" << std::endl;
		}
		assert(_allocationsCount == 0 && _usedMemory == 0);

		_start = nullptr;
		_size = 0;
	}


	virtual void* allocate(size_t size, u8 alignment = 4) = 0;
	virtual void deallocate(void* p) = 0;
	virtual void clear() = 0;

	const void* getStart() const
	{
		return _start;
	}

	size_t getSize() const
	{
		return _size;
	}

	size_t getUsedMemory() const
	{
		return _usedMemory;
	}

	size_t getNumAllocations() const
	{
		return _allocationsCount;
	}
};
