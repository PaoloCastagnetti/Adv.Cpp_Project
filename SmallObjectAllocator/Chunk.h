#pragma once
#include <iostream>
#include <cassert>
#include <bitset>
// POD (Plain Old Data) structure
// Structure defined inside FixedAllocator

struct SMALLOBJECTALLOCATOR_API Chunk {
	//Parameters
	unsigned char* pData_;
	unsigned char firstAvailableBlock_; // holds the index of the first block available in this chunk
	unsigned char blocksAvailable_; // the number of blocks available in this chunk

	//Methods
	/**
	 * @brief Initializes the Chunk with a specified block size and number of blocks.
	 *
	 * This function initializes the 'Chunk' object by allocating memory for it.
	 * It checks that the provided block size and number of blocks are valid, and
	 * then allocates memory accordingly. If the allocation is successful, the
	 * 'Chunk' is reset to its initial state.
	 *
	 * @param[in] blockSize The size of each block in bytes.
	 * @param[in] blocks The number of blocks to allocate.
	 * @return True if initialization is successful, false otherwise.
	 */
	bool Init(std::size_t blockSize, unsigned char blocks);

	/**
	 * @brief Allocates a block of memory within the Chunk.
	 *
	 * This function allocates a block of memory with the specified block size
	 * from the 'Chunk' object if there are available blocks. It ensures that the
	 * 'Chunk' is not completely filled before allocating. If an allocation is
	 * successful, the function returns a pointer to the allocated block; otherwise,
	 * it returns nullptr.
	 *
	 * @param[in] blockSize The size of the block to allocate in bytes.
	 * @return A pointer to the allocated block or nullptr if allocation fails.
	 */
	void* Allocate(std::size_t blockSize);

	/**
	 * @brief Deallocates a previously allocated block of memory within the Chunk.
	 *
	 * This function deallocates a previously allocated block of memory with the
	 * specified block size from the 'Chunk' object. It performs various checks
	 * to ensure that the deallocation is valid, including alignment and truncation
	 * checks. If successful, it updates the 'Chunk' to reflect the deallocation.
	 *
	 * @param[in] p A pointer to the block of memory to deallocate.
	 * @param[in] blockSize The size of the block being deallocated in bytes.
	 */
	void Deallocate(void* pointer, std::size_t blockSize);

	/**
	 * @brief Resets the Chunk to its initial state with a new block size and number of blocks.
	 *
	 * This function resets the 'Chunk' object to its initial state by updating its
	 * properties with a new block size and number of blocks. It ensures that the
	 * provided block size and number of blocks are valid and that there's no overflow
	 * during the calculation. The function initializes the available blocks within
	 * the 'Chunk' and sets the first available block to the beginning of the memory.
	 *
	 * @param[in] blockSize The size of each block in bytes.
	 * @param[in] blocks The number of blocks to allocate.
	 */
	void Reset(::std::size_t blockSize, unsigned char blocks);

	/**
	 * @brief Releases the memory allocated by the Chunk.
	 *
	 * This function releases the memory previously allocated by the 'Chunk' object.
	 * It checks whether the memory pointer `pData_` is not null before performing
	 * the deallocation using `std::free()`. This is a necessary step to prevent
	 * memory leaks.
	 */
	void Release();

	/**
	 * @brief Checks if the Chunk's internal structure is corrupt.
	 *
	 * This function checks whether the internal structure of the 'Chunk' object
	 * is corrupt. It performs several checks, including the number of available
	 * blocks, block indexes, and circular references if requested.
	 *
	 * @param[in] numBlocks The total number of blocks that should be in the Chunk.
	 * @param[in] blockSize The size of each block in bytes.
	 * @param[in] checkIndexes Whether to check for circular references (default is false).
	 * @return True if the Chunk's structure is corrupt, false otherwise.
	 */
	bool IsCorrupt(unsigned char numBlocks, std::size_t blockSize, bool checkIndexes) const;

	/**
	 * @brief Checks if a specific block of memory is available in the Chunk.
	 *
	 * This function checks whether a specific block of memory, represented by the
	 * pointer 'p', is available within the 'Chunk'. It performs various checks to
	 * ensure the block's validity, including alignment and the presence of the block
	 * in the 'Chunk'. The function is designed to work with multiple blocks but
	 * currently ignores the 'numBlocks' parameter.
	 *
	 * @param[in] p A pointer to the block of memory to check for availability.
	 * @param[in] numBlocks The total number of blocks (currently unused).
	 * @param[in] blockSize The size of each block in bytes.
	 * @return True if the specified block of memory is available, false otherwise.
	 */
	bool IsBlockAvailable(void* p, unsigned char numBlocks, std::size_t blockSize) const;

	//Inline methods
	/**
	 * @brief Checks if a given pointer is within the memory chunk.
	 *
	 * This function determines whether the provided pointer falls within the memory
	 * chunk's boundaries. It does so by comparing the pointer's address with the
	 * start and end addresses of the chunk.
	 *
	 * @param[in] p The pointer to check.
	 * @param[in] chunkLength The length of the memory chunk in bytes.
	 * @return True if the pointer is within the chunk, false otherwise.
	 */
	inline bool HasBlock(void* p, std::size_t chunkLength) const;

	/**
	 * @brief Checks if a specific number of blocks are available.
	 *
	 * This function checks if the number of available blocks in the 'Chunk' object
	 * is equal to the specified 'numBlocks'. It returns true if the condition is
	 * met and false otherwise.
	 *
	 * @param[in] numBlocks The number of blocks to check for availability.
	 * @return True if the specified number of blocks are available, false otherwise.
	 */
	inline bool HasAvailable(unsigned char numBlocks) const;

	/**
	 * @brief Checks if the Chunk is completely filled.
	 *
	 * This function checks whether all the blocks in the 'Chunk' object are
	 * occupied, meaning that no blocks are available for use. It returns true
	 * if all blocks are occupied (i.e., no blocks available), and false otherwise.
	 *
	 * @return True if the Chunk is completely filled, false otherwise.
	 */
	inline bool IsFilled(void) const;
	
};