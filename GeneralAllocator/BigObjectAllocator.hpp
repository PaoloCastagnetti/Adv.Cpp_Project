#pragma once

#include "framework.h"

template <typename T>
class GENERALALLOCATOR_API BigObjectAllocator {
public:
    /*
        @brief
        Create a BigObjectAllocator
    */
    BigObjectAllocator();
    /*
        @brief
        Allocate the memory of type T of size n
        @param n the size to allocate
        @return the pointer to the memory
    */
    T* allocate(size_t n) {
        T* ptr = static_cast<T*>(malloc(sizeof(T) * n));
        if (ptr)
            return ptr;

        throw std::bad_alloc();
    }
    /*
        @brief
        Free the pointer of type T
        @param ptr the pointer to free
        @param n the size of the ptr
    */
    void deallocate(T* ptr, size_t n) {
        (void)n;
        free(ptr);
    }
};

template <typename T>
BigObjectAllocator<T>::BigObjectAllocator() = default;

template <typename T>
T* BigObjectAllocator<T>::allocate(size_t n) {
    T* ptr = static_cast<T*>(malloc(sizeof(T) * n));
    if (ptr)
        return ptr;

    throw std::bad_alloc();
}
template <typename T>
void BigObjectAllocator<T>::deallocate(T* ptr, size_t n) {
    (void)n;
    free(ptr);
}

