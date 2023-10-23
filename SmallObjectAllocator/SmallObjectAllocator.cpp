#include "pch.h"
#include "SmallObjectAllocator.h"

void* DefaultAllocator(std::size_t numBytes, bool doThrow);
void DefaultDeallocator(void* p);

#pragma region Inline functions
inline std::size_t SmallObjectAllocator::GetOffset(std::size_t numBytes, std::size_t alignment) const {
    const ::std::size_t alignExtra = alignment - 1;
    return (numBytes + alignExtra) / alignment;
}
inline std::size_t SmallObjectAllocator::GetMaxObjectSize() const {
    return maxSmallObjectSize_;
}
inline std::size_t SmallObjectAllocator::GetAlignment() const { return objectAlignSize_; }
#pragma endregion

SmallObjectAllocator::SmallObjectAllocator(std::size_t pageSize, std::size_t maxObjectSize, std::size_t objectAlignSize) :
    pool_(nullptr),
    maxSmallObjectSize_(maxObjectSize),
    objectAlignSize_(objectAlignSize) {

    assert(0 != objectAlignSize);
    const std::size_t allocCount = GetOffset(maxObjectSize, objectAlignSize);
    pool_ = new FixedAllocator[allocCount];
    for (::std::size_t i = 0; i < allocCount; ++i)
        pool_[i].Initialize((i + 1) * objectAlignSize, pageSize);
}

SmallObjectAllocator::~SmallObjectAllocator(void) {
    delete[] pool_;
}

bool SmallObjectAllocator::TrimExcessMemory(void) {
    bool found = false;
    const ::std::size_t allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    ::std::size_t i = 0;
    for (; i < allocCount; ++i) {
        if (pool_[i].TrimEmptyChunk())
            found = true;
    }
    for (i = 0; i < allocCount; ++i) {
        if (pool_[i].TrimChunkList())
            found = true;
    }

    return found;
}

void* SmallObjectAllocator::Allocate(::std::size_t numBytes, bool doThrow) {
    if (numBytes > GetMaxObjectSize())
        return DefaultAllocator(numBytes, doThrow);

    assert(nullptr != pool_);
    if (0 == numBytes) numBytes = 1;
    const ::std::size_t index = GetOffset(numBytes, GetAlignment()) - 1;
    const ::std::size_t allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    (void)allocCount;
    assert(index < allocCount);

    FixedAllocator& allocator = pool_[index];
    assert(allocator.BlockSize() >= numBytes);
    assert(allocator.BlockSize() < numBytes + GetAlignment());
    void* place = allocator.Allocate();

    if ((nullptr == place) && TrimExcessMemory())
        place = allocator.Allocate();
    return place;
}

void SmallObjectAllocator::Deallocate(void* p, ::std::size_t numBytes) {
    if (nullptr == p)
        return;
    if (numBytes > GetMaxObjectSize()) {
        DefaultDeallocator(p);
        return;
    }
    assert(nullptr != pool_);
    if (0 == numBytes) numBytes = 1;
    const ::std::size_t index = GetOffset(numBytes, GetAlignment()) - 1;
    const ::std::size_t allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    (void)allocCount;
    assert(index < allocCount);
    FixedAllocator& allocator = pool_[index];
    assert(allocator.BlockSize() >= numBytes);
    assert(allocator.BlockSize() < numBytes + GetAlignment());
    const bool found = allocator.Deallocate(p, nullptr);
    (void)found;
    assert(found);
}

void SmallObjectAllocator::Deallocate(void* p) {
    if (nullptr == p)
        return;
    assert(nullptr != pool_);
    FixedAllocator* pAllocator = nullptr;
    const ::std::size_t allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    Chunk* chunk = nullptr;

    for (::std::size_t ii = 0; ii < allocCount; ++ii) {
        chunk = const_cast<Chunk*>(pool_[ii].HasBlock(p));
        if (nullptr != chunk) {
            pAllocator = &pool_[ii];
            break;
        }
    }
    if (nullptr == pAllocator) {
        DefaultDeallocator(p);
        return;
    }

    assert(nullptr != chunk);
    const bool found = pAllocator->Deallocate(p, chunk);
    (void)found;
    assert(found);
}

bool SmallObjectAllocator::IsCorrupt(void) const {
    if (nullptr == pool_) {
        assert(false);
        return true;
    }
    if (0 == GetAlignment()) {
        assert(false);
        return true;
    }
    if (0 == GetMaxObjectSize()) {
        assert(false);
        return true;
    }
    const std::size_t allocCount = GetOffset(GetMaxObjectSize(), GetAlignment());
    for (std::size_t ii = 0; ii < allocCount; ++ii) {
        if (pool_[ii].IsCorrupt())
            return true;
    }
    return false;
}

void* DefaultAllocator(std::size_t numBytes, bool doThrow) {
    void* p = std::malloc(numBytes);
    if (doThrow && (nullptr == p))
        throw std::bad_alloc();
    return p;
}

void DefaultDeallocator(void* p) {
    std::free(p);
}