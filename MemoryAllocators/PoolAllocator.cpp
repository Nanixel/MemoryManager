#include "stdafx.h"
#include "./PoolAllocator.h"


void PoolAllocator::clear() {
	u8 adjustment = pointer_math::GetAdjustmentForAddress(_start, _objectAlignment);
	//calculate the number of objects that can fit in the total size of memory allocated
	size_t numberOfObjects = (size_t)floor((_size - adjustment) / _objectSize);

	//initialize the free_list
	_freeList = (void**)pointer_math::add(_start, adjustment);

	void** p = _freeList;

	for (size_t i = 0; i < numberOfObjects - 1; i++) {
		*p = pointer_math::add(p, _objectSize);
		p = (void**)*p;
	}

	*p = nullptr;
}


void* PoolAllocator::allocate(size_t memSize, u8 alignment) {

	// We only have to align on the first memory allocation.
	assert(memSize > 0 && "allocate called with memSize = 0.");
	assert(memSize == _objectSize && alignment == _objectAlignment);

	// There are no free spaces in the memory pool.
	if (_freeList == nullptr) { 
		return nullptr;
	}

	void* p = _freeList;

	// At this point we dereference the current memory address which then is going to be the next free slot that points to
	// the free slot after it (which then is derefference in the next call to this and so on..)
	_freeList = (void**)(*_freeList);

	_usedMemory += memSize;
	_allocationsCount++;

	return p;
}

void PoolAllocator::deallocate(void* p) {
	*((void**)p) = _freeList;

	_freeList = (void**)p;

	_usedMemory -= _objectSize;
	_allocationsCount--;
}

