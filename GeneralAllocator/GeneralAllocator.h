#pragma once
#include "framework.h"
#include <string>
#include <vector>
#include <iostream>
#include "RedBlackTree.hpp"
#include <cstddef>

class FreeBlock {
public:
    char* address;
    size_t size;
    FreeBlock* pre;
    FreeBlock* next;
    FreeBlock() : pre(nullptr), next(nullptr) {}
    ~FreeBlock() {
        if (next)
            delete next;
    }
};

class GENERALALLOCATOR_API GeneralAllocator {
public:
    /*
        @brief
        create the block of memory, the RedBlackTree and the list
        @param totalSize the total size of the memory block
    */
    GeneralAllocator(size_t totalSize);
    /*
        @brief
        free all the resources
    */
    ~GeneralAllocator();

    /*
        @brief
        create a block of the specific size using a RedBlackTree to search the first free node and modify the node
        @param size the size of the block
        @return the void pointer to the block
    */
    void* Allocate(size_t size);
    /*
        @brief
        free the block of a certain size and add or modify the corrisponding node in the RedBlackTree
        @param size the size of the block
    */
    void Deallocate(void* ptr, size_t n);
private:
    /*
        @brief
        delete the node on the list and modify the RedBlackTree
        @param address the searched address
    */
    void DeleteNodeOnList(char* address);
    /*
        @brief
        modify the node on the list and modify the RedBlackTree
        @param oldAddress the searched address
        @param address the new address
        @param newSize the new size
    */
    void ModifyNodeOnList(char* oldAddress, char* address, size_t newSize);
    /*
        @brief
        add the node on the list and modify the RedBlackTree
        @param address the address
        @param newSize the size
    */
    void AddNodeOnList(char* address, size_t size);
    /*
        @brief
        add the size to an axisting node and change the address if necessary
        @param nodeOnList the node on the list
        @param address the address of the node to merge
        @param size the size of the node to merge
    */
    void MergeNodeOnList(FreeBlock* nodeOnList, char* address, size_t size);
    /*
        @brief
        add the size to an axisting node and change the address if necessary considering also the next node on the list
        @param nodeOnList the node on the list
        @param address the address of the node to merge
        @param size the size of the node to merge
        @param nodeNextOnList the next node on the list
    */
    void MergeNodeOnList(FreeBlock* nodeOnList, char* address, size_t size, FreeBlock* nodeNextOnList);
    size_t size;
    size_t totalSize;
    char* memoryBlock;
    FreeBlock* firstNode;
    RedBlackTree<char*, size_t>* freeRBtree;
};