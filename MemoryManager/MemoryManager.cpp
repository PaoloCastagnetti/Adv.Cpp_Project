#include "MemoryManager.h"

void MemoryManager::Init()
{
	bigObjAllocator = new BigObjectAllocator(_bigObjTotalSize);
	smallObjAllocator = new SmallObjectAllocator(_smallObjPageSize, _smallObjTotalSize, _smallObjAlignSize);
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
		// DEFAULT: do NOT throw exceptions
		bool doThrow = true;
		smallObjAllocator->Allocate(size, !doThrow);
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

template<class NewT>
NewT* MemoryManager::MM_New()
{
	// In this case it utilizes the BigObjectAllocator
	if (size >= _sizeThreshold)
	{
		NewT newPtr = (NewT*)bigObjAllocator->Allocate(sizeof(NewT));
		new(newPtr);

		return newPtr;
	}

	// In this case it utilizes the SmallObjectAllocator
	else
	{
		NewT newPtr = (NewT*)smallObjAllocator->Allocate(sizeof(NewT));
		new(newPtr);

		return newPtr;
	}
}

template<class DeleteT>
void MM_Delete(DeleteT* ptr)
{
	// Call the default destructor for type DeleteT
	ptr->~DeleteT();

	// In this case it utilizes the BigObjectAllocator
	if (size >= _sizeThreshold)
	{
		bigObjAllocator->Deallocate(ptr, sizeof(NewT));
	}

	// In this case it utilizes the SmallObjectAllocator
	else
	{
		smallObjAllocator->Deallocate(ptr, sizeof(NewT));
	}

	// Make the pointer null
	ptr = nullptr;
}

// TODO
template<class NewT>
NewT* MM_New_A(std::size_t array_size)
{

}

// TODO
template<class DeleteT>
void MM_Delete_A(DeleteT* ptr)
{

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