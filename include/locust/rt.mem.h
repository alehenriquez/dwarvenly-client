#define MEM_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

void
nonnullfunc
mem_free(ptr p);


void 
nonnullfunc
mem_copy_(ptr from, ptr to, uint64 len, char const * const func_name, char const * const file_name, int line);


void
overloaded nonnullfunc
mem_set_(ptr dest, uint64 len, var val, char const * const func_name, char const * const file_name, int line);

void
static inlinefunc overloaded nonnullfunc
mem_set_(ptr dest, uint64 len, uint64 val, char const * const func_name, char const * const file_name, int line) {
	var v = {.type = & META_OF(uint64), .data = & val };
	mem_set_(dest, len, v, func_name, file_name, line);
}

ptr
overloaded nonnullfunc mustuseretfunc mallocfunc mallocsizefunc(1)
mem_get(uint64 len, meta const * const m);

ptr
static inlinefunc overloaded mustuseretfunc mallocfunc mallocsizefunc(1)
mem_get(uint64 len) {
	return mem_get(len, & META_OF(byte));
}

ptr
overloaded nonnullfunc mustuseretfunc mallocsizefunc(2)
mem_resize(ptr p, uint64 len, meta const * const m);

ptr 
static inlinefunc overloaded nonnullfunc mustuseretfunc mallocsizefunc(2)
mem_resize(ptr p, uint64 len) {
	return mem_resize(p, len, & META_OF(byte));
}

#define mem_set(dest, len, val) mem_set_(dest, len, val, __func__, __FILE__, __LINE__)
#define mem_copy(from, to, len) mem_copy_(from, to, len, __func__, __FILE__, __LINE__)
