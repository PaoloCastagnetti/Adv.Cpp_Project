#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include "Chunk.h"

class SMALLOBJECTALLOCATOR_API FixedAllocator {

public:
	//Constructors

	/**
	 * @brief Default constructor for the FixedAllocator class.
	 *
	 * This constructor initializes the FixedAllocator object with default values for its member variables.
	 * These variables include the block size, the number of blocks, pointers to chunks, and allocation and deallocation chunk pointers.
	 */
    FixedAllocator();

	/**
	 * @brief Destructor for the FixedAllocator class.
	 *
	 * This destructor is responsible for releasing the allocated memory chunks owned by the FixedAllocator object.
	 * It iterates through all the chunks in the chunks_ container and calls the Release() method on each of them to release memory.
	 */
    ~FixedAllocator();

	//Methods

	/**
	 * @brief Initialize the FixedAllocator object with a given block size and page size.
	 *
	 * This method configures the FixedAllocator with the specified block size and page size.
	 * It also validates that the provided values are within certain limits and sets the internal state accordingly.
	 *
	 * @param blockSize The size of each memory block in bytes.
	 * @param pageSize The size of each memory page in bytes.
	 */
    void Initialize(std::size_t blockSize, std::size_t pageSize);

	/**
	 * @brief Allocate a memory block from the FixedAllocator.
	 *
	 * This method allocates a memory block from the FixedAllocator.
	 * It manages the allocation process by selecting an appropriate chunk
	 * to allocate from or creating a new chunk if needed.
	 *
	 * @return A pointer to the allocated memory block, or nullptr if allocation fails.
	 */
    void* Allocate(void);

	/**
	 * @brief Deallocate a memory block in the FixedAllocator.
	 *
	 * This method deallocates a memory block within the FixedAllocator. It manages the deallocation process
	 * by finding the appropriate chunk to deallocate from based on the given hint or the memory address.
	 *
	 * @param p A pointer to the memory block to deallocate.
	 * @param hint A hint to guide the deallocation process, or nullptr if no hint is provided.
	 * @return true if the deallocation is successful, false otherwise.
	 */
    bool Deallocate(void* p, Chunk* hint);

	/**
	 * @brief Trim an empty memory chunk from the FixedAllocator.
	 *
	 * This method removes an empty memory chunk from the FixedAllocator if one exists.
	 * It ensures that only one empty chunk can exist at a time and manages the pointers accordingly.
	 *
	 * @return true if an empty chunk is successfully trimmed, false otherwise.
	 */
    bool TrimEmptyChunk(void);

	/**
	 * @brief Trim the chunk list of the FixedAllocator to match its size to capacity.
	 *
	 * This method adjusts the size of the chunk list to match its capacity. It ensures that the
	 * allocation and deallocation pointers are updated correctly after resizing the list.
	 *
	 * @return true if the chunk list is successfully trimmed, false otherwise.
	 */
    bool TrimChunkList(void);

	/**
	 * @brief Count the number of empty chunks in the FixedAllocator.
	 *
	 * This method counts the number of empty chunks in the FixedAllocator.
	 * Empty chunks are those with no available memory blocks for allocation.
	 *
	 * @return The number of empty chunks (0 or 1).
	 */
    std::size_t CountEmptyChunks(void) const;

	/**
	 *  @brief Checks if the FixedAllocator is corrupted.
	 *
	 * @return Boolean value indicating whether the FixedAllocator is corrupted.
	 */
    bool IsCorrupt(void) const;

	/**
	 * @brief Checks if the given memory address is within the range of any Chunk in the FixedAllocator.
	 *
	 * @param p Pointer to the memory address to be checked.
	 * @return Pointer to the Chunk if the memory address is found within its range, otherwise nullptr.
	 */
    const Chunk* HasBlock(void* p) const;

	/**
	 * @biref Retrieves the size of individual blocks managed by the FixedAllocator.
	 *
	 * @return The size of individual blocks in the FixedAllocator.
	 */
	inline std::size_t BlockSize() const;

private:

	//Methods

	/**
	 * @brief Deallocates the memory block pointed to by the given address.
	 *
	 * @param p Pointer to the memory address to deallocate.
	 */
	void DoDeallocate(void* p);
	/**
	 * @brief Creates a new Chunk in the FixedAllocator.
	 *
	 * @return Boolean value indicating the success of creating a new Chunk.
	 */
	bool MakeNewChunk(void);
	/**
	 * @brief Finds the Chunk in the vicinity of the given memory address.
	 *
	 * @param p Pointer to the memory address.
	 * @return Pointer to the Chunk if found in the vicinity, otherwise nullptr.
	 */
	Chunk* VicinityFind(void* p) const;

	//Parameters

	// Number of bytes in a single block within a Chunk.
	std::size_t blockSize_;

	// Number of blocks managed by each Chunk.
	unsigned char numBlocks_;

	// Fewest # of objects managed by a Chunk.
	static unsigned char MinObjectsPerChunk_;

	// Most # of objects managed by a Chunk - never exceeds UCHAR_MAX.
	static unsigned char MaxObjectsPerChunk_;
	
	// Type of container used to hold Chunks.
	typedef std::vector< Chunk > Chunks;

	// Iterator through container of Chunks.
	typedef Chunks::iterator ChunkIter;

	// Iterator through const container of Chunks.
	typedef Chunks::const_iterator ChunkCIter;

	// Container of Chunks.
	Chunks chunks_;

	// Pointer to Chunk used for last or next allocation.
	Chunk* allocChunk_;

	// Pointer to Chunk used for last or next deallocation.
	Chunk* deallocChunk_;

	// Pointer to the only empty Chunk if there is one, else nullptr.
	Chunk* emptyChunk_;
};