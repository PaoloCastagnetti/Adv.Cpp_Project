#pragma once

#include "framework.h"
#include "FixedAllocator.h"
#include <string>
#include <vector>

class SMALLOBJECTALLOCATOR_API SmallObjectAllocator {
	public:
		SmallObjectAllocator(std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize);

		~SmallObjectAllocator();

		bool TrimExcessMemory(void);

		void* Allocate(std::size_t numBytes, bool doThrow);

		void Deallocate(void* p);

		void Deallocate(void* p, ::std::size_t numBytes);

		bool IsCorrupt(void) const;

		inline std::size_t GetAlignment() const;

		inline std::size_t GetMaxObjectSize() const;

		inline std::size_t GetOffset(std::size_t numBytes, std::size_t alignment) const;

	private:
		FixedAllocator * pool_;

		// Largest object size supported by allocators.
		const std::size_t maxSmallObjectSize_;

		// Size of alignment boundaries.
		const std::size_t objectAlignSize_;
};