#pragma once


#include "./BaseAllocator.h"


class PoolAllocator : public BaseAllocator {

private:
	size_t _objectSize;
	u8 _objectAlignment;
	void** _freeList;
	PoolAllocator(const PoolAllocator&);
	PoolAllocator& operator=(const PoolAllocator&);

public:

	PoolAllocator(size_t size, void* start, size_t fixedElementSize, u8 objectAlignment) : BaseAllocator(size, start), 
		_objectSize(fixedElementSize), _objectAlignment(objectAlignment) {
		clear();
	}

	~PoolAllocator() {
		_freeList = nullptr;
	}

	void* allocate(size_t size, u8 alignment) override;
	void deallocate(void* p) override;
	void clear() override;

};