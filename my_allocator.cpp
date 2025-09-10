#include "my_allocator.h"
#include "block_header.h"
#include <sys/mman.h>
#include <cstddef>
#include <iostream>
#include <cstring>

static MemoryBlock* head = nullptr;
static const size_t INITIAL_POOL_SIZE = 1024 * 1024; // 1 MB initial pool

// Function to find a suitable free block (First Fit Strategy)
static MemoryBlock* find_free_block(size_t size) {
    MemoryBlock* current = head;
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Function to split a block if it's too large
static void split_block(MemoryBlock* block, size_t size) {
    if (block->size > size + sizeof(MemoryBlock)) {
        MemoryBlock* new_block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(block) + sizeof(MemoryBlock) + size);
        new_block->size = block->size - size - sizeof(MemoryBlock);
        new_block->is_free = true;
        new_block->next = block->next;
        new_block->prev = block;
        block->size = size;
        block->next = new_block;
        
        if (new_block->next) {
            new_block->next->prev = new_block;
        }
    }
}

// Function to merge adjacent free blocks
static void coalesce_blocks() {
    MemoryBlock* current = head;
    while (current) {
        if (current->is_free && current->next && current->next->is_free) {
            current->size += current->next->size + sizeof(MemoryBlock);
            current->next = current->next->next;
            if (current->next) {
                current->next->prev = current;
            }
            // Don't advance 'current' after a merge, check the new next block.
        } else {
            current = current->next;
        }
    }
}

// Function to initialize the allocator with a large block of memory
void init_allocator() {
    void* initial_memory = mmap(NULL, INITIAL_POOL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (initial_memory == MAP_FAILED) {
        std::cerr << "Initial memory allocation failed." << std::endl;
        return;
    }
    head = static_cast<MemoryBlock*>(initial_memory);
    head->size = INITIAL_POOL_SIZE - sizeof(MemoryBlock);
    head->is_free = true;
    head->next = nullptr;
    head->prev = nullptr;
}

// The main allocation function
void* my_malloc(size_t size) {
    if (head == nullptr) {
        init_allocator();
    }
    
    MemoryBlock* block = find_free_block(size);

    if (block) {
        if (block->size > size + sizeof(MemoryBlock)) {
            split_block(block, size);
        }
        block->is_free = false;
        return reinterpret_cast<void*>(reinterpret_cast<char*>(block) + sizeof(MemoryBlock));
    }
    
    // Allocate a new block from the OS if the pool is full
    void* new_memory = mmap(NULL, size + sizeof(MemoryBlock), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_memory == MAP_FAILED) {
        return nullptr;
    }
    
    MemoryBlock* new_block = static_cast<MemoryBlock*>(new_memory);
    new_block->size = size;
    new_block->is_free = false;
    new_block->next = head;
    new_block->prev = nullptr;
    if (head) {
        head->prev = new_block;
    }
    head = new_block;
    
    return reinterpret_cast<void*>(reinterpret_cast<char*>(new_block) + sizeof(MemoryBlock));
}

// The main deallocation function
void my_free(void* ptr) {
    if (!ptr) {
        return;
    }
    MemoryBlock* block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(ptr) - sizeof(MemoryBlock));
    block->is_free = true;
    coalesce_blocks();
}

// my_calloc implementation
void* my_calloc(size_t num, size_t size) {
    size_t total_size = num * size;
    void* ptr = my_malloc(total_size);
    if (ptr) {
        memset(ptr, 0, total_size);
    }
    return ptr;
}

// my_realloc implementation
void* my_realloc(void* ptr, size_t size) {
    if (ptr == nullptr) {
        return my_malloc(size);
    }
    if (size == 0) {
        my_free(ptr);
        return nullptr;
    }

    MemoryBlock* old_block_header = reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(ptr) - sizeof(MemoryBlock));
    size_t old_size = old_block_header->size;

    if (size <= old_size) {
        if (old_size - size >= sizeof(MemoryBlock) + 1) { // 1 byte to prevent empty block
            split_block(old_block_header, size);
            my_free(reinterpret_cast<void*>(reinterpret_cast<char*>(old_block_header) + sizeof(MemoryBlock) + size));
        }
        return ptr;
    }

    void* new_ptr = my_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_size);
        my_free(ptr);
    }
    return new_ptr;
}