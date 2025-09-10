#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <cstddef>

void* my_malloc(size_t size);
void my_free(void* ptr);

// Add these two lines
void* my_calloc(size_t num, size_t size);
void* my_realloc(void* ptr, size_t size);

#endif