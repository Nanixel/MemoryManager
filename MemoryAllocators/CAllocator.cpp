#include "stdafx.h"
#include "./CAllocator.h"

void* CAllocator::allocate(size_t size, u8 alignment) {
	return malloc(size);
}

void CAllocator::deallocate(void* memory) {
	free(memory);
}

void CAllocator::clear() {

}