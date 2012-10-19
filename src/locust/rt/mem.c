#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Roots
ptr overloaded mem_get(uint64 num) {
    return malloc(num);
}

ptr overloaded mem_get (const meta *m, uint64 num) {
    assert(m != nil);
    if (m != nil) {
        num *= m->size;
    }
    return malloc(num);
}

ptr overloaded mem_resize(ptr location, uint64 num) {
    assert(location != nil);
    return realloc(location, num);
}

ptr overloaded mem_resize(ptr location, const meta *m, uint64 num) {
    assert(location != nil);
    assert(m != nil);
    if (m != nil) {
        num *= m->size;
    }
    return realloc(location, num);
}

void mem_free(ptr location) {
    assert(location != nil);
    free(location);
}

// TODO: port beyond x86_64 if I ever need to.
// Not a public api call because coders are all retards, including me. If you want it, use C.
static inline void __mem_copy_unaliased(ptr restrict from, ptr restrict to, uint64 count) {
	__asm__ volatile("cld; rep; movsq;"
		:"=D" (to), "=S" (from)
		:"0" (to), "1" (from), "c" (count >> 3)
		:"memory");
	__asm__ volatile(" cld; rep; movsb;"
		:"=D" (to), "=S" (from)
		:"0" (to), "1" (from), "c" (count & 7)
		:"memory");
	return;
}

// TODO: port beyond x86_64 if I ever need to.
void mem_copy(ptr from, ptr to, uint64 count) {
    assert(from != nil);
    assert(to != nil);

    // no overlap
    if (to < from || to > from+count) {
        __mem_copy_unaliased(from, to, count);
        return;
    }

    // no copy needed (y u do dis...)
    if (to == from || count == 0) {
        return;
    }

    // overlap :(
    ptr from_end = from + count;
    ptr to_end = to + count;
    uint64 ol_offset = to - from;
    uint64 ol_count = count - ol_offset;

    // backwards first because of overlap
    __asm__ volatile("std; rep; movsq"
    :"=D" (to_end), "=S" (from_end)
    :"0" (to_end), "1" (from_end), "c" (ol_count >> 3)
    :"memory");
    __asm__ volatile("std; rep; movsb"
    :"=D" (to_end), "=S" (from_end)
    :"0" (to_end), "1" (from_end), "c" (ol_count & 7)
    :"memory");

    // then frontwards for the remainder
    __mem_copy_unaliased(from, to, ol_offset);

    return;
}

void mem_set(ptr location, uint64 len, byte value) {
    assert(location != nil);
    assert(len != 0);
    __asm__ __volatile__(
        // tiny lengths get tiny loops. :D
        "cmp $0x7, %[len];\n"
        "jl 7f;\n"

        // loop until cache aligned
        "0:\n"
        "   movq %[loc], %%rcx;\n"
        "   andq $0x7, %%rcx;\n"
        "   subq %%rcx, %[len];\n"
        "1:\n"
        "   jrcxz 2f;\n"
        "   decq %%rcx;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 1b;\n"


        // aligned cache looper
        "2:\n"
        // construct cache line sized source since we're sure we need it now
        "   movb %%dl, %%dh;\n"
        "   movw %%dx, %%cx;\n"
        "   shlq $16, %%rcx;\n"
        "   movw %%dx, %%cx;\n"
        "   movl %%ecx, %%edx;\n"
        "   shlq $32, %%rdx;\n"
        "   movl %%ecx, %%edx;\n"

        "   movq %[len], %%rcx;\n"
        "   shrq $4, %%rcx;\n"
        "3:\n"
        "   jrcxz 4f;\n"
        "   decq %%rcx;\n"
        "   movq %%rdx, (%[loc]);\n"
        "   addq $0x8, %[loc];\n"
        "   jmp 3b;\n"

        "4:\n"
        "   movq %[len], %%rcx;\n"
        "   andq $0x7, %%rcx;\n"
        "5:\n"
        "   jrcxz 9f;\n"
        "   decq %%rcx;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 5b;\n"

        "6:\n"
        "   movb %b[len], %%cl;\n"
        "7:\n"
        "   jrcxz 9f;\n"
        "   decb %%cl;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 7b;\n"

        "9:\n"
        :
        : [loc]"Q"(location), [len]"Q"(len), "d"(value)
        : "rcx"
    );
}


// lookups for random memory location to type therein, at highest level
typedef struct {
    uintptr_t start;
    size_t len;
    int8 type;
} mem_range_desc;

/*
typedef struct {
    void *prev_phys;
    void *prev_free;
    void *next_free;
    uint32 size;
} free_block;

typedef struct {
    void *prev_phys;
    void *next_phys;
    type_desc *type;
    uint32 size;
} used_block;

#define BLOCK_SIZE 4096

void* block_get() {
    void *p;
    uint32 retries = RETRY_COUNT;
    while (retries-->0) {
        p = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
        if (p == MAP_FAILED) {
            fprintf(stderr, "block_get() error: %s", strerror(errno));
        }
    }
    return (p == MAP_FAILED) ? NULL : p;
}

void block_release(void *p) {
    if (munmap(p, BLOCK_SIZE) != 0) {
        fprintf(stderr, "block_release() error: %s", strerror(errno));
    }
    return;
}


typedef struct {
    uint32 len;
    void *frees;
} free_list;

typedef struct {
    uint64 free_mask;
    free_list *free_lists;
} size_list;


void mem_init() {


}
*/


