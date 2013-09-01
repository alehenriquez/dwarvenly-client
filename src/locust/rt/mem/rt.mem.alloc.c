#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif


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

