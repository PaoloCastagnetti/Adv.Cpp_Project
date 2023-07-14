/**
 * @file Chunk.cpp
 * @brief Implementation file for the Chunk class.
 */

#include "Chunk.h"

 /**
  * @brief Initializes the Chunk with the specified block size and number of blocks.
  * Allocates memory for the blocks and sets up the block allocation list.
  * @param blockSize The size of each block in bytes.
  * @param blocks The number of blocks in the Chunk.
  */
void Chunk::Init(std::size_t blockSize, unsigned char blocks) {
	pointerData_ = new unsigned char[blockSize * blocks];
	firstAvailableBlock_ = 0;
	blocksAvailable_ = blocks;
	unsigned char* p = pointerData_;
	for (unsigned char i = 0; i != blocks; p += blockSize) {
		*p = ++i;
	}
}

/**
 * @brief Allocates a block of memory from the Chunk.
 * If there are no available blocks, returns nullptr.
 * @param blockSize The size of the block to allocate in bytes.
 * @return A pointer to the allocated block, or nullptr if no blocks are available.
 */
void* Chunk::Allocate(std::size_t blockSize) {
	if (!blocksAvailable_) {
		return 0;
	}
	unsigned char* pResult = pointerData_ + (firstAvailableBlock_ * blockSize);
	// Update firstAvailableBlock_ to point to the next block
	firstAvailableBlock_ = *pResult;
	--blocksAvailable_;
	return pResult;
}

/**
 * @brief Deallocates a block of memory in the Chunk and makes it available for reuse.
 * @param p A pointer to the block to deallocate.
 * @param blockSize The size of the block in bytes.
 */
void Chunk::Deallocate(void* p, std::size_t blockSize) {
	assert(p >= pointerData_);
	unsigned char* toRelease = static_cast<unsigned char*>(p);
	// Alignment check
	assert((toRelease - pointerData_) % blockSize == 0);
	*toRelease = firstAvailableBlock_;
	firstAvailableBlock_ = static_cast<unsigned char>((toRelease - pointerData_) / blockSize);
	// Truncation check
	assert(firstAvailableBlock_ == (toRelease - pointerData_) / blockSize);
	++blocksAvailable_;
}