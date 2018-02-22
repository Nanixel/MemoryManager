#pragma once
#include "./MemoryChunkAllocator.h"


#define COMPONENT_T_CHUNK_SIZE = 512

template<class T>
class ComponentContainer : public Memory::MemoryChunkAllocator<T, 512> {
	ComponentContainer(ComponentContainer&) = delete;
	ComponentContainer& operator=(ComponentContainer&) = delete;

public:
	ComponentContainer() : MemoryChunkAllocator("ComponentManager") {

	}

	virtual ~ComponentContainer() {

	}
};


