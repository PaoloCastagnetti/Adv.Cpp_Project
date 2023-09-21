#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include "Chunk.h"

class FixedAllocator {

public:

    FixedAllocator();

    ~FixedAllocator();

    void Initialize(std::size_t blockSize, std::size_t pageSize);

    void* Allocate(void);

    bool Deallocate(void* p, Chunk* hint);

	inline std::size_t BlockSize();

    bool TrimEmptyChunk(void);

    bool TrimChunkList(void);

    std::size_t CountEmptyChunks(void) const;

    bool IsCorrupt(void) const;

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