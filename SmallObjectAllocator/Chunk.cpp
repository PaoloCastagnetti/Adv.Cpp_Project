#include "Chunk.h"

#pragma region Inline functions

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

#pragma endregion

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

void* Chunk::Allocate(std::size_t blockSize) {
	if (IsFilled())
		return nullptr;
	assert((firstAvailableBlock_ * blockSize) / blockSize == firstAvailableBlock_);
	unsigned char* pResult = pData_ + (firstAvailableBlock_ * blockSize);
	firstAvailableBlock_ = *pResult;
	--blocksAvailable_;

	return pResult;
}

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

void Chunk::Reset(std::size_t blockSize, unsigned char blocks) {
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

bool Chunk::IsCorrupt(unsigned char numBlocks, ::std::size_t blockSize, bool checkIndexes) const {

    if (numBlocks < blocksAvailable_) {
        assert(false);
        return true;
    }
    if (IsFilled())
        return false;
    unsigned char index = firstAvailableBlock_;
    if (numBlocks <= index) {
        assert(false);
        return true;
    }
    if (!checkIndexes)
        return false;

    std::bitset< UCHAR_MAX > foundBlocks;
    unsigned char* nextBlock = nullptr;

    for (unsigned char cc = 0; ; ) {
        nextBlock = pData_ + (index * blockSize);
        foundBlocks.set(index, true);
        ++cc;
        if (cc >= blocksAvailable_)
            break;
        index = *nextBlock;
        if (numBlocks <= index) {
            assert(false);
            return true;
        }
        if (foundBlocks.test(index)) {
            assert(false);
            return true;
        }
    }
    if (foundBlocks.count() != blocksAvailable_) {
        assert(false);
        return true;
    }

    return false;
}

bool Chunk::IsBlockAvailable(void* p, unsigned char numBlocks, std::size_t blockSize) const {
    (void)numBlocks;

    if (IsFilled())
        return false;

    unsigned char* place = static_cast<unsigned char*>(p);
    assert((place - pData_) % blockSize == 0);
    unsigned char blockIndex = static_cast<unsigned char>(
        (place - pData_) / blockSize);

    unsigned char index = firstAvailableBlock_;
    assert(numBlocks > index);
    if (index == blockIndex)
        return true;

    ::std::bitset< UCHAR_MAX > foundBlocks;
    unsigned char* nextBlock = nullptr;
    for (unsigned char cc = 0; ; ) {
        nextBlock = pData_ + (index * blockSize);
        foundBlocks.set(index, true);
        ++cc;
        if (cc >= blocksAvailable_)
            break;
        index = *nextBlock;
        if (index == blockIndex)
            return true;
        assert(numBlocks > index);
        assert(!foundBlocks.test(index));
    }

    return false;
}