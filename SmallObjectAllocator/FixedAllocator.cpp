/**
 * @file FixedAllocator.cpp
 * @brief Implementation file for the FixedAllocator class.
 */

#include "FixedAllocator.h"

 /**
  * @brief Allocates a block of memory from the FixedAllocator.
  * If there are no available blocks in the current chunk, a new chunk is added.
  * @return A pointer to the allocated block of memory.
  */
void* FixedAllocator::Allocate(){
	if (allocChunk_ == 0 ||
		allocChunk_->blocksAvailable_ == 0) {
		// No available memory in the current chunk
		// Try to find a chunk with available memory
		Chunks::iterator i = chunks_.begin();
		for (;; ++i) {
			if (i == chunks_.end()) {
				// All chunks are filled up - add a new chunk
				chunks_.reserve(chunks_.size() + 1);
				Chunk newChunk;
				newChunk.Init(blockSize_, numBlocks_);
				chunks_.push_back(newChunk);
				allocChunk_ = &chunks_.back();
				deallocChunk_ = &chunks_.back();
				break;
			}
			if (i->blocksAvailable_ > 0) {
				// Found a chunk with available memory
				allocChunk_ = &*i;
				break;
			}
		}
	}
	assert(allocChunk_ != 0);
	assert(allocChunk_->blocksAvailable_ > 0);
	return allocChunk_->Allocate(blockSize_);
}