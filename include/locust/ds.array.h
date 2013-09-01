#define ARRAY_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

// x86_64 cache line size ... usually...
#define CACHE_LINE_SIZE 64

#define array__type_id ((type_id)27)
extern TYPE_ID_DECL(array);
extern META_DECL(array);

typedef ptr array;

array
static inline nonnullfunc
array_create(meta const * const type, uint64 count) {
	// arrays look like
	// typedef struct {
	//     meta const * type;
    //     uint64 len;
    //  ->  byte contents[n];
    //     ptr front;
	// } array;
	// where n is the length of the array in bytes
	// and an array* is &contents[0], which front also is.
	// so length and meta data are behind the ptr
	ptr ret;
	count = type->size * count;
	error_goto_if((ret = mem_get(count + sizeof(meta const * const) + sizeof(uint64) + sizeof(ptr))) == nil, "NilPtr: ret; mem_get() failure");
	*((meta const **)ret) = type;
	ret += sizeof(meta const * const);
	*((uint64*)ret) = count;
	ret += sizeof(uint64);
	*((ptr*)(ret+count)) = ret;
	return ret;
  error:
  	return nil;
}

static inline overloaded nonnullfunc
meta const *
array_type(array self) {
	return *(meta const * const *)(self - (sizeof(uint64) + sizeof(meta const * const)));
}

uint64
static inline overloaded nonnullfunc
array_len(array self) {
	return *(uint64*)(self - sizeof(uint64));
}

ptr
static inline overloaded nonnullfunc
array_index(array self, uint64 i) {
	i *= array_type(self)->size;
	i %= array_len(self);
	return (ptr)(((uintptr_t)self) + i );
}

ptr 
static inline overloaded nonnullfunc
array_index(array self, int64 i) {
	uint64 ii = (__myabs(i) * array_type(self)->size) % array_len(self);
	if (i < 0) {
		ii = array_len(self) - ii;
	}
    return (ptr)(((uintptr_t)self) + ii);
}
