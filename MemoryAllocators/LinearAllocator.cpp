#include "stdafx.h"

#include "./LinearAllocator.h"
#include <cassert>



LinearAllocator::LinearAllocator(size_t size, void* start)
	: BaseAllocator(size, start)
{
	
}

LinearAllocator::~LinearAllocator()
{
	clear();
}

void* LinearAllocator::allocate(size_t size, u8 alignment)
{
	assert(size > 0 && "allocate called with memSize = 0.");

	union
	{
		void* asVoidPtr;
		uintptr_t asUptr;
	};

	asVoidPtr = (void*)this->_start;
	asUptr += this->_usedMemory;

	u8 adjustment = pointer_math::GetAdjustmentForAddress(asVoidPtr, alignment);

	if (_usedMemory + adjustment + size > _size)
		return nullptr;

	asUptr += adjustment;

	_usedMemory += size + adjustment;
	_allocationsCount++;

	return asVoidPtr;
}

void LinearAllocator::deallocate(void* p)
{
	assert(false && "Lineaer allocators do not support free operations. Use clear instead.");
}

void LinearAllocator::clear()
{
	_allocationsCount = 0;
	_usedMemory = 0;

}