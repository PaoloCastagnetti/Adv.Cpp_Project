#pragma once

#include "framework.h"
#include "FixedAllocator.h"
#include <string>
#include <vector>

class SMALLOBJECTALLOCATOR_API SmallObjectAllocator {
	public:

		/**
		 * @brief Constructs a SmallObjectAllocator with the specified parameters.
		 *
		 * @param pageSize The size of the memory page.
		 * @param maxObjectSize The maximum size of the small objects.
		 * @param objectAlignSize The alignment size of the objects.
		 */
		SmallObjectAllocator(std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize);

		/**
		 * @brief Destructor for SmallObjectAllocator.
		 */
		~SmallObjectAllocator();

		/**
		 * @brief Trims excess memory in the SmallObjectAllocator's pool.
		 *
		 * @return Boolean value indicating whether excess memory was found and trimmed.
		 */
		bool TrimExcessMemory(void);

		/**
		 * @brief Allocates memory of the specified size in the SmallObjectAllocator.
		 *
		 * @param numBytes The size of the memory to allocate.
		 * @param doThrow A boolean indicating whether to throw an exception on allocation failure.
		 * @return Pointer to the allocated memory if successful, otherwise nullptr.
		 */
		void* Allocate(std::size_t numBytes, bool doThrow);

		/**
		 * @brief Deallocates memory at the specified memory address in the SmallObjectAllocator.
		 *
		 * @param p Pointer to the memory address to deallocate.
		 */
		void Deallocate(void* p);

		/**
		 * @brief Deallocates memory at the specified memory address in the SmallObjectAllocator.
		 *
		 * @param p Pointer to the memory address to deallocate.
		 * @param numBytes The size of the memory to deallocate.
		 */
		void Deallocate(void* p, ::std::size_t numBytes);

		/**
		 * @brief Checks if the SmallObjectAllocator is corrupted.
		 *
		 * @return Boolean value indicating whether the SmallObjectAllocator is corrupted.
		 */
		bool IsCorrupt(void) const;

		/**
		 * @biref Retrieves the alignment value used by the SmallObjectAllocator.
		 *
		 * @return The alignment value used by the SmallObjectAllocator.
		 */
		inline std::size_t GetAlignment() const;

		/**
		 * @brief Retrieves the maximum size of small objects managed by the SmallObjectAllocator.
		 *
		 * @return The maximum size of small objects.
		 */
		inline std::size_t GetMaxObjectSize() const;

		/**
		 * @brief Calculates the offset for the given number of bytes and alignment.
		 *
		 * @param numBytes The number of bytes.
		 * @param alignment The alignment value.
		 * @return The offset calculated based on the provided parameters.
		 */
		inline std::size_t GetOffset(std::size_t numBytes, std::size_t alignment) const;

	private:
		FixedAllocator * pool_;

		// Largest object size supported by allocators.
		const std::size_t maxSmallObjectSize_;

		// Size of alignment boundaries.
		const std::size_t objectAlignSize_;
};