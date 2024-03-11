#pragma once
#include<stdio.h>
#include "BigObjectAllocator.h"
#include "SmallObjectAllocator.h"


class MemoryManager {
public:
	// no need for constructor in this class
	MemoryManager() = delete;

	// Instantiate SmallObjectAllocator and BigObjectAllocator
	void Init();

	// Free SmallObjectAllocator and BigObjectAllocator from memory
	void Free();

	void* Allocate(std::size_t size);
	void Deallocate(void* ptr, std::size_t size);

	size_t GetSizeThreshold();
	void SetSizeThreshold(std::size_t sizeThreshold);

	template<class NewT>
	NewT* MM_New()
	{
		// In this case it utilizes the BigObjectAllocator
		if (size >= _sizeThreshold)
		{
			NewT newPtr = (NewT*) bigObjAllocator->Allocate(sizeof(NewT));
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

private:
	BigObjectAllocator* bigObjAllocator;

	// WARNING: MISSING SMALL OBJ ALLOCATOR REFERENCE
	BigObjectAllocator* smallObjAllocator;

	std::size_t _sizeThreshold;
	std::size_t _objTotalSize;
};


// macro definition

#ifndef MEMORY_MANAGER_MACROS
#define MEMORY_MANAGER_MACROS

// Allocate a block of raw memory
#   define MM_MALLOC(size) ::MemoryManager::Allocate(size)
// Free the memory allocated with MM_MALLOC
#   define MM_FREE(ptr, size) ::MemoryManager::Deallocate(ptr, size)

// new / delete overrides
#	define MM_NEW_T(T) ::MemoryManager::MM_New<T>()
#	define MM_DELETE_T(T, ptr) ::MemoryManager::MM_Delete<T>(ptr)

// new / delete array overrides
#	define MM_NEW_ARRAY_T(T, array_size) ::MemoryManager::MM_New_A<T>(array_size)
#	define MM_DELETE_ARRAY_T(T, ptr) ::MemoryManager::MM_Delete_A<T>(ptr)

#endif