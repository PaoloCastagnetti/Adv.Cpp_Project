#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include "Chunk.h"

class FixedAllocator {

public:
    /// Create a FixedAllocator which manages blocks of 'blockSize' size.
    FixedAllocator();

    /// Destroy the FixedAllocator and release all its Chunks.
    ~FixedAllocator();

    /// Initializes a FixedAllocator by calculating # of blocks per Chunk.
    void Initialize(std::size_t blockSize, std::size_t pageSize);

    /** Returns pointer to allocated memory block of fixed size - or nullptr
     if it failed to allocate.
     */
    void* Allocate(void);

    /** Deallocate a memory block previously allocated with Allocate.  If
     the block is not owned by this FixedAllocator, it returns false so
     that SmallObjAllocator can call the default deallocator.  If the
     block was found, this returns true.
     */
    bool Deallocate(void* p, Chunk* hint);

    /// Returns block size with which the FixedAllocator was initialized.
    inline std::size_t BlockSize() const { return blockSize_; }

    /** Releases the memory used by the empty Chunk.  This will take
     constant time under any situation.
     @return True if empty chunk found and released, false if none empty.
     */
    bool TrimEmptyChunk(void);

    /** Releases unused spots from ChunkList.  This takes constant time
     with respect to # of Chunks, but actual time depends on underlying
     memory allocator.
     @return False if no unused spots, true if some found and released.
     */
    bool TrimChunkList(void);

    /** Returns count of empty Chunks held by this allocator.  Complexity
     is O(C) where C is the total number of Chunks - empty or used.
     */
    std::size_t CountEmptyChunks(void) const;

    /** Determines if FixedAllocator is corrupt.  Checks data members to
     see if any have erroneous values, or violate class invariants.  It
     also checks if any Chunk is corrupt.  Complexity is O(C) where C is
     the number of Chunks.  If any data is corrupt, this will return true
     in release mode, or assert in debug mode.
     */
    bool IsCorrupt(void) const;

    /** Returns true if the block at address p is within a Chunk owned by
     this FixedAllocator.  Complexity is O(C) where C is the total number
     of Chunks - empty or used.
     */
    const Chunk* HasBlock(void* p) const;
	


private:

	void DoDeallocate(void* p);
	bool MakeNewChunk(void);
	Chunk* VicinityFind(void* p) const;

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