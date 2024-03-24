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
void MemoryManager::MM_Delete(DeleteT* ptr)
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


template<class NewT>
NewT* MemoryManager::MM_New_A(std::size_t array_size)
{
	std::size_t arrayBytes = array_size * sizeof(char);

	// Accounting for metadata size (1) and "size_t" size
	arrayBytes += 1 + sizeof(std::size_t);

	// Using BigObjectAllocator
	if (arrayBytes >= _sizeThreshold)
	{
		unsigned char* ptr = (unsigned char*) bigObjAllocator->Allocate(arrayBytes);

		// Metadata
		*(std::size_t*) ptr = array_size;
		*(ptr + 1) = sizeof(std::size_t);

		NewT* newPtr = (NewT*) (ptr + 1 sizeof(std::size_t));

		for (std::size_t i = 0; i < array_size; ++i)
		{
			newPtr[i] = NewT();
		}

		return newPtr;
	}

	// Using SmallObjectAllocator
	else
	{
		unsigned char* ptr = (unsigned char*) smallObjAllocator->Allocate(arrayBytes);

		// Metadata
		*(std::size_t*) ptr = array_size;
		*(ptr + 1) = 1;

		NewT* newPtr = (NewT*) (ptr + 1 sizeof(std::size_t));

		for (std::size_t i = 0; i < array_size; ++i)
		{
			newPtr[i] = NewT();
		}

		return newPtr;
	}
}


template<class DeleteT>
void MemoryManager::MM_Delete_A(DeleteT* ptr)
{
	// Take array length from metadata
	unsigned char metadataSize = *((std::size_t*)ptr);

	ptr = ptr - 1 - metadataSize; // Points now to the actual array

	std::size_t arrayBytes = sizeof(DeleteT) * metadataSize + 1 + sizeof(std::size_t);

	for (std::size_t i = 0; i < length; ++i)
	{
		pointer[i].~DeleteT();
	}

	// Using BigObjectAllocator
	if (arrayBytes >= _sizeThreshold)
	{
		smallObjAllocator->Deallocate(ptr, arrayBytes);
	}

	// Using SmallObjectAllocator
	else
	{
		bigObjAllocator->Deallocate(ptr, arrayBytes);
	}

	pointer = nullptr;
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