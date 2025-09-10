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

// New function to test my_calloc and my_realloc
void test_calloc_and_realloc() {
    std::cout << "--- Starting calloc and realloc Test ---" << std::endl;

    // Test my_calloc()
    std::cout << "Testing my_calloc: Allocating 5 integers and checking for zero-initialization." << std::endl;
    int* zero_array = static_cast<int*>(my_calloc(5, sizeof(int)));
    if (zero_array) {
        bool is_zero = true;
        for (int i = 0; i < 5; ++i) {
            if (zero_array[i] != 0) {
                is_zero = false;
                break;
            }
        }
        if (is_zero) {
            std::cout << "SUCCESS: my_calloc correctly zero-initialized memory." << std::endl;
        } else {
            std::cerr << "FAILURE: my_calloc did not zero-initialize memory." << std::endl;
        }
        my_free(zero_array);
    }
    
    std::cout << std::endl;

    // Test my_realloc()
    std::cout << "Testing my_realloc: resizing a block." << std::endl;
    int* old_array = static_cast<int*>(my_malloc(5 * sizeof(int)));
    for (int i = 0; i < 5; ++i) {
        old_array[i] = i + 1;
    }
    std::cout << "Original array allocated: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << old_array[i] << " ";
    }
    std::cout << std::endl;

    int* new_array = static_cast<int*>(my_realloc(old_array, 10 * sizeof(int)));
    if (new_array) {
        std::cout << "SUCCESS: my_realloc successfully resized the block." << std::endl;
        std::cout << "Resized array (first 5 elements should be the same): ";
        for (int i = 0; i < 5; ++i) {
            std::cout << new_array[i] << " ";
        }
        std::cout << std::endl;
        my_free(new_array);
    } else {
        std::cerr << "FAILURE: my_realloc failed to resize the block." << std::endl;
        my_free(old_array); // Free the original block if realloc failed
    }

    std::cout << "--- calloc and realloc Test Complete ---" << std::endl;
    std::cout << std::endl;
}

int main() {
    test_simple_allocation();
    test_fragmentation_and_coalescing();
    test_calloc_and_realloc();
    return 0;
}