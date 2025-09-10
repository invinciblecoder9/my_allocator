#ifndef BLOCK_HEADER_H
#define BLOCK_HEADER_H

#include <cstddef>

struct MemoryBlock {
    size_t size;         // Size of the memory block
    bool is_free;        // True if the block is free, false otherwise
    MemoryBlock* next;   // Pointer to the next block in the linked list
    MemoryBlock* prev;   // Pointer to the previous block in the linked list
};

#endif