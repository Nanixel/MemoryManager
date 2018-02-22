#pragma once
#include "./BaseAllocator.h"

class LinearAllocator : public BaseAllocator
{
public:
	LinearAllocator(size_t size, void* start);
	~LinearAllocator();

	void* allocate(size_t size, u8 alignment) override;

	void deallocate(void* p) override;

	void clear() override;


private:
	LinearAllocator(const LinearAllocator&);
	LinearAllocator& operator=(const LinearAllocator&);

};