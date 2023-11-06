#include "pch.h"
#include "BigObjectAllocator.h"

BigObjectAllocator::BigObjectAllocator(size_t totalSize) {
    memoryBlock = new char[totalSize];
    freeRBtree = new RedBlackTree<char*, size_t>();
    freeRBtree->Insert(memoryBlock, totalSize);
    FreeBlock* f = new FreeBlock();
    f->address = memoryBlock;
    f->size = totalSize;
    firstNode = f;
    this->totalSize = totalSize;
}

BigObjectAllocator::~BigObjectAllocator() {
    delete[] memoryBlock;
    delete freeRBtree;
    delete firstNode;
}

void* BigObjectAllocator::Allocate(size_t size) {
    Node<char*, size_t>* freeBlockNode = freeRBtree->Search(size);

    if (freeBlockNode) {
        if (freeBlockNode->val < size)
            return nullptr;
        char* blockStart = freeBlockNode->key;

        if (freeBlockNode->val > size) {
            char* newStart = blockStart + size;
            size_t newSize = freeBlockNode->val - size;

            ModifyNodeOnList(blockStart, newStart, newSize);
        }
        else {
            DeleteNodeOnList(blockStart);
        }

        return blockStart;
    }
    else {
        return nullptr;
    }
}

void BigObjectAllocator::Deallocate(void* ptr, size_t size) {
    char* cptr = static_cast<char*>(ptr);
    if (memoryBlock <= cptr && memoryBlock + totalSize >= cptr && memoryBlock <= cptr + size && memoryBlock + totalSize >= cptr + size)
        AddNodeOnList(cptr, size);
}

void BigObjectAllocator::DeleteNodeOnList(char* address) {
    FreeBlock* curr = firstNode;
    if (curr == nullptr) return;
    while (curr->address != address) {
        if (curr->next == nullptr) return;
        curr = curr->next;
    }

    freeRBtree->Remove(curr->address, curr->size);

    curr->pre->next = curr->next;
    curr->next->pre = curr->pre;
    delete curr;
}
void BigObjectAllocator::ModifyNodeOnList(char* oldAddress, char* address, size_t newSize) {
    FreeBlock* curr = firstNode;
    if (curr == nullptr) return;
    while (curr->address != oldAddress) {
        if (curr->next == nullptr) return;
        curr = curr->next;
    }
    freeRBtree->Remove(curr->address, curr->size);
    curr->size = newSize;
    curr->address = address;
    freeRBtree->Insert(curr->address, curr->size);
}
void BigObjectAllocator::AddNodeOnList(char* address, size_t size) {
    FreeBlock* curr = firstNode;
    if (curr == nullptr) {
        firstNode = new FreeBlock();
        firstNode->address = address;
        firstNode->size = size;
        return;
    }

    while (curr->address < address) {
        if (curr->next == nullptr) {
            if (curr->address + curr->size == address) {
                MergeNodeOnList(curr, address, size);
            }
            else {
                FreeBlock* newNode = new FreeBlock();
                newNode->address = address;
                newNode->size = size;
                newNode->pre = curr;
                newNode->next = nullptr;
                freeRBtree->Insert(address, size);
            }
            return;
        }
        curr = curr->next;
    }
    if (curr->pre != nullptr) {
        FreeBlock* pre = curr->pre;
        if (pre->address + pre->size == address) {

            if (pre->next != nullptr && pre->next->address == pre->address + pre->size + size) {
                MergeNodeOnList(pre, address, size, pre->next);
            }
            else {
                MergeNodeOnList(pre, address, size);
            }
        }
        else if (address + size == curr->address) {
            MergeNodeOnList(curr, address, size);
        }
        else {
            freeRBtree->Insert(address, size);

            FreeBlock* newNode = new FreeBlock();
            newNode->address = address;
            newNode->size = size;
            newNode->pre = pre;
            newNode->next = curr;

            curr->pre = newNode;
            pre->next = newNode;
        }
    }
    else {
        if (address + size == curr->address) {
            MergeNodeOnList(curr, address, size);
        }
        else {
            freeRBtree->Insert(address, size);

            FreeBlock* newNode = new FreeBlock();
            newNode->address = address;
            newNode->size = size;
            newNode->pre = nullptr;
            newNode->next = curr;

            curr->pre = newNode;
            firstNode = newNode;
        }

    }
}

void BigObjectAllocator::MergeNodeOnList(FreeBlock* nodeOnList, char* address, size_t size) {
    FreeBlock* newNode = new FreeBlock();
    freeRBtree->Remove(nodeOnList->address, nodeOnList->size);
    if (nodeOnList->address > address) {
        nodeOnList->address = address;
    }
    nodeOnList->size = nodeOnList->size + size;

    freeRBtree->Insert(nodeOnList->address, nodeOnList->size);
}
void BigObjectAllocator::MergeNodeOnList(FreeBlock* nodeOnList, char* address, size_t size, FreeBlock* nodeNextOnList) {
    FreeBlock* newNode = new FreeBlock();
    freeRBtree->Remove(nodeOnList->address, nodeOnList->size);
    freeRBtree->Remove(nodeNextOnList->address, nodeNextOnList->size);
    nodeOnList->size = nodeOnList->size + size + nodeNextOnList->size;
    nodeNextOnList->next->pre = nodeOnList;
    nodeOnList->next = nodeNextOnList->next;
    delete nodeNextOnList;
    freeRBtree->Insert(nodeOnList->address, nodeOnList->size);
}