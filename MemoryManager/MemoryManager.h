#pragma once
#include<stdio.h>
#include "BigObjectAllocator.h"
#include "SmallObjectAllocator.h"

// MACRO definition
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


class MemoryManager {
public:
	// no need for constructor in this class

	// Instantiate SmallObjectAllocator and BigObjectAllocator
	void Init();

	// Free SmallObjectAllocator and BigObjectAllocator from memory
	void Free();

	void* Allocate(std::size_t size);
	void Deallocate(void* ptr, std::size_t size);

	template<class NewT>
	NewT* MM_New();

	template<class DeleteT>
	void MM_Delete(DeleteT* ptr);

	template<class NewT>
	NewT* MM_New_A(std::size_t array_size);

	template<class DeleteT>
	void MM_Delete_A(DeleteT* ptr);

	std::size_t GetSizeThreshold();
	void SetSizeThreshold(std::size_t sizeThreshold);

private:
	BigObjectAllocator* bigObjAllocator;

	SmallObjectAllocator* smallObjAllocator;

	std::size_t _sizeThreshold;

	std::size_t _bigObjTotalSize;

	std::size_t	_smallObjPageSize;
	std::size_t _smallObjTotalSize;
	std::size_t _smallObjAlignSize;
};