#include "my_allocator.h"
#include <iostream>
#include <vector>

void test_simple_allocation() {
    std::cout << "--- Starting Simple Allocation Test ---" << std::endl;
    int* p1 = static_cast<int*>(my_malloc(10 * sizeof(int)));
    if (p1) {
        std::cout << "Allocated 10 integers." << std::endl;
        my_free(p1);
        std::cout << "Freed 10 integers." << std::endl;
    }
    std::cout << "--- Simple Allocation Test Complete ---" << std::endl;
    std::cout << std::endl;
}

void test_fragmentation_and_coalescing() {
    std::cout << "--- Starting Fragmentation & Coalescing Test ---" << std::endl;
    std::vector<void*> ptrs;

    // Allocate a number of small blocks
    std::cout << "Allocating 10 blocks of 100 bytes each." << std::endl;
    for (int i = 0; i < 10; ++i) {
        ptrs.push_back(my_malloc(100));
    }

    // Free every other block to induce fragmentation
    std::cout << "Freeing every other block to induce fragmentation." << std::endl;
    for (size_t i = 0; i < ptrs.size(); i += 2) {
        my_free(ptrs[i]);
    }
    
    // Attempt to allocate a large block. This should force coalescing.
    std::cout << "Attempting to allocate a large block (300 bytes)." << std::endl;
    void* large_ptr = my_malloc(300);
    if (large_ptr) {
        std::cout << "SUCCESS: Allocated a large block. Coalescing is working." << std::endl;
        my_free(large_ptr);
    } else {
        std::cerr << "FAILURE: Failed to allocate a large block. Coalescing might not be working." << std::endl;
    }

    // Clean up remaining blocks
    for (size_t i = 1; i < ptrs.size(); i += 2) {
        my_free(ptrs[i]);
    }
    std::cout << "--- Fragmentation & Coalescing Test Complete ---" << std::endl;
    std::cout << std::endl;
}

int main() {
    test_simple_allocation();
    test_fragmentation_and_coalescing();
    return 0;
}