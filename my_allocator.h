#include <cstddef>

#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

void* my_malloc(size_t size);
void my_free(void* ptr);

#endif