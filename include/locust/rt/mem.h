#define MEM_H

#ifndef TYPES_H
#include <types/types.h>
#endif

// Roots
ptr overloaded mem_get (uint64 number);
ptr overloaded mem_get (const meta *tm, uint64 number);
ptr overloaded mem_resize(ptr location, uint64 num);
ptr overloaded mem_resize(ptr location, const meta *tm, uint64 num);
void mem_free(ptr location);
void mem_copy(ptr from, ptr to, uint64 count);
void mem_set(ptr location, uint64 count, byte value);
