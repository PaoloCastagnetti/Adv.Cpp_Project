#include "MemoryManager.h"

void MemoryManager::Init()
{
	bigObjAllocator = new BigObjectAllocator(_objTotalSize);
	smallObjAllocator = new BigObjectAllocator(_objTotalSize);
}

void MemoryManager::Free()
{
	free(bigObjAllocator);
	free(smallObjAllocator);
}

void* MemoryManager::Allocate(std::size_t size)
{
	// In this case it utilizes the BigObjectAllocator
	if (size >= _sizeThreshold)
	{
		bigObjAllocator->Allocate(size);
	}

	// In this case it utilizes the SmallObjectAllocator
	else
	{
		smallObjAllocator->Allocate(size);
	}
}

void MemoryManager::Deallocate(void* ptr, std::size_t size)
{
	// In this case it utilizes the BigObjectAllocator
	if (size >= _sizeThreshold)
	{
		bigObjAllocator->Deallocate(ptr, size);
	}

	// In this case it utilizes the SmallObjectAllocator
	else
	{
		smallObjAllocator->Deallocate(ptr, size);
	}
}


// SIZE_THRESHOLD GETTER & SETTER

size_t MemoryManager::GetSizeThreshold()
{
	return _sizeThreshold;
}

void MemoryManager::SetSizeThreshold(size_t sizeThreshold)
{
	_sizeThreshold = sizeThreshold;
}