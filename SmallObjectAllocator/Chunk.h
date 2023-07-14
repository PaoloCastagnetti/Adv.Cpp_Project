#pragma once
#include <iostream>
#include <cassert>
// POD (Plain Old Data) structure
// Structure defined inside FixedAllocator

struct Chunk {

	//Parameters
	unsigned char* pointerData_;
	unsigned char firstAvailableBlock_; // holds the index of the first block available in this chunk
	unsigned char blocksAvailable_; // the number of blocks available in this chunk

	//Methods
	void Init(std::size_t blockSize, unsigned char blocks);
	void* Allocate(std::size_t blockSize);
	void Deallocate(void* pointer, std::size_t blockSize);
	
};