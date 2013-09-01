#define RING_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#define CACHE_LINE_SIZE 64

typedef struct {
	// cache line 1
    uint64 r_counter; // read
    byte __pad1[CACHE_LINE_SIZE - sizeof(uint64)];

    // cache line 2
    uint64 w_counter; // write
    byte __pad2[CACHE_LINE_SIZE - sizeof(uint64)];

    // cache line 3
    uint64 a_counter; // acquire -- writers use to claim space. they update w_count when done.
    byte __pad3[CACHE_LINE_SIZE - sizeof(uint64)*3];
    const meta* items_desc;
    uint64 cap;

    // cache line 4
    byte items[];
} ring;

#define ring__type_id ((type_id)30)
extern TYPE_ID_DECL(ring);
extern META_DECL(ring);

void ring_print(ring *self);

void ring_init(ring *self, const meta *type, uint64 cap);
void ring_deinit(ring *self);

bool ring_is_full(ring *self);
bool ring_is_empty(ring *self);

bool ring_write(ring *self, const meta *type, ptr item);
bool ring_read(ring *self, const meta *type, ptr item);

