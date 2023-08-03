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
bool Chunk::Init(std::size_t blockSize, unsigned char blocks) {
	assert(blockSize > 0);
	assert(blocks > 0);

	std::size_t allocSize = blockSize * blocks;
	assert(allocSize / blockSize == blocks);

	pData_ = static_cast<unsigned char*>(::std::malloc(allocSize));
	if (nullptr == pData_)
		return false;

	Reset(blockSize, blocks);
	return true;
}

/**
 * @brief Allocates a block of memory from the Chunk.
 * If there are no available blocks, returns nullptr.
 * @param blockSize The size of the block to allocate in bytes.
 * @return A pointer to the allocated block, or nullptr if no blocks are available.
 */
void* Chunk::Allocate(std::size_t blockSize) {
	if (IsFilled())
		return nullptr;
	assert((firstAvailableBlock_ * blockSize) / blockSize == firstAvailableBlock_);
	unsigned char* pResult = pData_ + (firstAvailableBlock_ * blockSize);
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
	assert(p >= pData_);
	unsigned char* toRelease = static_cast<unsigned char*>(p);
	// Alignment check
	assert((toRelease - pData_) % blockSize == 0);
	unsigned char index = static_cast<unsigned char>(
		(toRelease - pData_) / blockSize);

	if (0 < blocksAvailable_)
		assert(firstAvailableBlock_ != index);

	*toRelease = firstAvailableBlock_;
	firstAvailableBlock_ = index;
	// Truncation check
	assert(firstAvailableBlock_ == (toRelease - pData_) / blockSize);

	++blocksAvailable_;
}

void Chunk::Reset(::std::size_t blockSize, unsigned char blocks) {
	assert(blockSize > 0);
	assert(blocks > 0);
	// Overflow check
	assert((blockSize * blocks) / blockSize == blocks);

	firstAvailableBlock_ = 0;
	blocksAvailable_ = blocks;

	unsigned char i = 0;
	for (unsigned char* p = pData_; i != blocks; p += blockSize) {
		*p = ++i;
	}
}
void Chunk::Release() {
	assert(nullptr != pData_);
	std::free(static_cast<void*>(pData_));
}

inline bool Chunk::HasBlock(void* p, ::std::size_t chunkLength) const {
	unsigned char* pc = static_cast<unsigned char*>(p);
	return (pData_ <= pc) && (pc < pData_ + chunkLength);
}

inline bool Chunk::HasAvailable(unsigned char numBlocks) const {
	return (blocksAvailable_ == numBlocks);
}

inline bool Chunk::IsFilled(void) const {
	return (0 == blocksAvailable_);
}

bool Chunk::IsCorrupt(unsigned char numBlocks, ::std::size_t blockSize,
    bool checkIndexes) const {

    if (numBlocks < blocksAvailable_) {
        // Contents at this Chunk corrupted.  This might mean something has
        // overwritten memory owned by the Chunks container.
        assert(false);
        return true;
    }
    if (IsFilled())
        // Useless to do further corruption checks if all blocks allocated.
        return false;
    unsigned char index = firstAvailableBlock_;
    if (numBlocks <= index) {
        // Contents at this Chunk corrupted.  This might mean something has
        // overwritten memory owned by the Chunks container.
        assert(false);
        return true;
    }
    if (!checkIndexes)
        // Caller chose to skip more complex corruption tests.
        return false;

    /* If the bit at index was set in foundBlocks, then the stealth index was
     found on the linked-list.
     */
    std::bitset< UCHAR_MAX > foundBlocks;
    unsigned char* nextBlock = nullptr;

    /* The loop goes along singly linked-list of stealth indexes and makes sure
     that each index is within bounds (0 <= index < numBlocks) and that the
     index was not already found while traversing the linked-list.  The linked-
     list should have exactly blocksAvailable_ nodes, so the for loop will not
     check more than blocksAvailable_.  This loop can't check inside allocated
     blocks for corruption since such blocks are not within the linked-list.
     Contents of allocated blocks are not changed by Chunk.

     Here are the types of corrupted link-lists which can be verified.  The
     corrupt index is shown with asterisks in each example.

     Type 1: Index is too big.
      numBlocks == 64
      blocksAvailable_ == 7
      firstAvailableBlock_ -> 17 -> 29 -> *101*
      There should be no indexes which are equal to or larger than the total
      number of blocks.  Such an index would refer to a block beyond the
      Chunk's allocated domain.

     Type 2: Index is repeated.
      numBlocks == 64
      blocksAvailable_ == 5
      firstAvailableBlock_ -> 17 -> 29 -> 53 -> *17* -> 29 -> 53 ...
      No index should be repeated within the linked-list since that would
      indicate the presence of a loop in the linked-list.
     */
    for (unsigned char cc = 0; ; ) {
        nextBlock = pData_ + (index * blockSize);
        foundBlocks.set(index, true);
        ++cc;
        if (cc >= blocksAvailable_)
            // Successfully counted off number of nodes in linked-list.
            break;
        index = *nextBlock;
        if (numBlocks <= index) {
            /* This catches Type 1 corruptions as shown in above comments.
             This implies that a block was corrupted due to a stray pointer
             or an operation on a nearby block overran the size of the block.
             */
            assert(false);
            return true;
        }
        if (foundBlocks.test(index)) {
            /* This catches Type 2 corruptions as shown in above comments.
             This implies that a block was corrupted due to a stray pointer
             or an operation on a nearby block overran the size of the block.
             Or perhaps the program tried to delete a block more than once.
             */
            assert(false);
            return true;
        }
    }
    if (foundBlocks.count() != blocksAvailable_) {
        /* This implies that the singly-linked-list of stealth indexes was
         corrupted.  Ideally, this should have been detected within the loop.
         */
        assert(false);
        return true;
    }

    return false;
}

bool Chunk::IsBlockAvailable(void* p, unsigned char numBlocks,
    ::std::size_t blockSize) const {
    (void)numBlocks;

    if (IsFilled())
        return false;

    unsigned char* place = static_cast<unsigned char*>(p);
    // Alignment check
    assert((place - pData_) % blockSize == 0);
    unsigned char blockIndex = static_cast<unsigned char>(
        (place - pData_) / blockSize);

    unsigned char index = firstAvailableBlock_;
    assert(numBlocks > index);
    if (index == blockIndex)
        return true;

    /* If the bit at index was set in foundBlocks, then the stealth index was
     found on the linked-list.
     */
    ::std::bitset< UCHAR_MAX > foundBlocks;
    unsigned char* nextBlock = nullptr;
    for (unsigned char cc = 0; ; ) {
        nextBlock = pData_ + (index * blockSize);
        foundBlocks.set(index, true);
        ++cc;
        if (cc >= blocksAvailable_)
            // Successfully counted off number of nodes in linked-list.
            break;
        index = *nextBlock;
        if (index == blockIndex)
            return true;
        assert(numBlocks > index);
        assert(!foundBlocks.test(index));
    }

    return false;
}