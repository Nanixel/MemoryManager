#pragma once
#include "BaseAllocator.h"

class CAllocator : public BaseAllocator {
private:
	CAllocator(const CAllocator&);
	CAllocator& operator=(const CAllocator&);
public:

	CAllocator(size_t size, void* start) : BaseAllocator(size, start) {	}
	~CAllocator() {}

	void* allocate(size_t size, u8 alignment) override;
	void deallocate(void* memory) override;
	void clear();


};