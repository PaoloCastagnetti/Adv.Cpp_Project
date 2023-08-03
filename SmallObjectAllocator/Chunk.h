#pragma once
#include <iostream>
#include <cassert>
#include <bitset>
// POD (Plain Old Data) structure
// Structure defined inside FixedAllocator

struct Chunk {
	//Parameters
	unsigned char* pData_;
	unsigned char firstAvailableBlock_; // holds the index of the first block available in this chunk
	unsigned char blocksAvailable_; // the number of blocks available in this chunk
	inline bool HasBlock(void* p, ::std::size_t chunkLength) const;

	//Methods
	bool Init(std::size_t blockSize, unsigned char blocks);
	void* Allocate(std::size_t blockSize);
	void Deallocate(void* pointer, std::size_t blockSize);
	void Reset(::std::size_t blockSize, unsigned char blocks);
	void Release();
	bool IsCorrupt(unsigned char numBlocks, std::size_t blockSize, bool checkIndexes) const;
	bool IsBlockAvailable(void* p, unsigned char numBlocks, std::size_t blockSize) const;

	inline bool HasBlock(void* p, ::std::size_t chunkLength) const;
	inline bool HasAvailable(unsigned char numBlocks) const;
	inline bool IsFilled(void) const;
	
};