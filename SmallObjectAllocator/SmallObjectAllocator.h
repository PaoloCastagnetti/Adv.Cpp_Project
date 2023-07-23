#pragma once

#include "framework.h"
#include "FixedAllocator.h"
#include <string>
#include <vector>

class SMALLOBJECTALLOCATOR_API SmallObjectAllocator {
	public:
		SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize);
		void* Allocate(std::size_t numBytes);
		void Deallocate(void* p, std::size_t size);

	private:
		std::vector<FixedAllocator> pool_;
		FixedAllocator* pLastAlloc_;
		FixedAllocator* pLastDealloc_;
};