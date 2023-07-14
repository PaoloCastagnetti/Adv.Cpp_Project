#pragma once
#include <iostream>
#include <vector>
#include "Chunk.h"

class FixedAllocator {

public:
	void* Allocate();
	void Deallocate();

private:

	// Blocks
	std::size_t blockSize_;
	unsigned char numBlocks_;
	
	// Chunks
	typedef std::vector<Chunk> Chunks;
	Chunks chunks_;
	Chunk* allocChunk_;
	Chunk* deallocChunk_;
};