#pragma once

#include "./BaseAllocator.h"

class StackAllocator : public BaseAllocator {
private:

	struct AllocationMetaInfo {
		u8 adjustment;
	};

	struct AllocationHeader
	{
		u8 adjustment;
	};

	StackAllocator(const StackAllocator&);
	StackAllocator& operator=(const StackAllocator&);

public:

	StackAllocator(size_t size, void* start) : BaseAllocator(size, start) {}
	~StackAllocator();

	void* allocate(size_t size, u8 alignment = 4) override;
	void deallocate(void* p) override;

	void clear() override;

	void deallocate2(void* p);
	void* allocate2(size_t size, u8 alignment = 4);

};