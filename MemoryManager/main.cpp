#include <stdio.h>
#include "MemoryManager.h"

int main() {
    // Initialize MemoryManager
    MemoryManager memoryManager;
    memoryManager.Init();

    // Allocate memory
    int* intPtr = static_cast<int*>(memoryManager.Allocate(sizeof(int)));
    if (intPtr) {
        *intPtr = 42;
        printf("Allocated int: %d\n", *intPtr);
    }
    else {
        printf("Memory allocation failed!\n");
    }

    //// Deallocate memory
    //memoryManager.Deallocate(intPtr, sizeof(int));

    //// Demonstrate new / delete overrides
    //int* newIntPtr = memoryManager.MM_NEW_T(int);
    //if (newIntPtr) {
    //    *newIntPtr = 100;
    //    printf("Allocated int using MM_NEW_T: %d\n", *newIntPtr);
    //    memoryManager.MM_DELETE_T(int, newIntPtr);
    //}
    //else {
    //    printf("Memory allocation using MM_NEW_T failed!\n");
    //}

    //// Demonstrate new / delete array overrides
    //int* newArrayIntPtr = memoryManager.MM_NEW_ARRAY_T(int, 5);
    //if (newArrayIntPtr) {
    //    printf("Allocated array of int using MM_NEW_ARRAY_T: ");
    //    for (int i = 0; i < 5; ++i) {
    //        newArrayIntPtr[i] = i;
    //        printf("%d ", newArrayIntPtr[i]);
    //    }
    //    printf("\n");
    //    memoryManager.MM_DELETE_ARRAY_T(int, newArrayIntPtr);
    //}
    //else {
    //    printf("Memory allocation using MM_NEW_ARRAY_T failed!\n");
    //}

    // Free MemoryManager
    memoryManager.Free();

    return 0;
}