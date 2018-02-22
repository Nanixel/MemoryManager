#include "stdafx.h"
#include "./StackAllocator.h"



StackAllocator::~StackAllocator() {	
	clear();
}

///This works the same as the linear allocator with the exception that we now adjust with metadata that holds and ajustmentvalue
///This way we now how much extra bytes to deallocate for any given object that needed an adjustment in order to be aligned
//return a pointer memory on top of the stack with allocated space of "size"
void* StackAllocator::allocate(size_t size, u8 alignment) {
	assert(size > 0 && "allocate called with memSize = 0.");

	union
	{
		void* asVoidPtr;
		uintptr_t asUptr;
		AllocationMetaInfo* asMeta;
	};
	
	asVoidPtr = (void*)this->_start; 
	asUptr += this->_usedMemory; 

	//Make any needed adjustments to align the new requested memory block "size"
	u8 adjustment = pointer_math::GetAdjustmentForAddress(asVoidPtr, alignment, sizeof(AllocationMetaInfo));

	if (_usedMemory + adjustment + size > _size)
		return nullptr;

	asMeta->adjustment = adjustment; //Save the adjustment to the metadata
	asUptr += adjustment; //add any adjustment to the currently availible address to align the memory

	_usedMemory += size + adjustment;
	_allocationsCount++;

	return asVoidPtr;
}

//passes in a pointer to a memory address
void StackAllocator::deallocate(void *p) {
	
	union
	{
		void* asVoidPtr;
		uintptr_t asUptr;
		AllocationMetaInfo* asMeta;
	};

	asVoidPtr = (void*)this->_start; //get the start of the allocator's memory pool
	asUptr += this->_usedMemory;

	AllocationHeader* header = (AllocationHeader*)(pointer_math::subtract(p, sizeof(AllocationHeader)));

	_usedMemory -= asUptr - (uintptr_t)p + header->adjustment;

	_allocationsCount--;
}

void StackAllocator::clear() {
	_usedMemory = 0;
	_allocationsCount = 0;
}
