#define RING_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct {
    uint64 cap;
    uint64 r_counter; // read
    uint64 w_counter; // write
    uint64 a_counter; // acquire -- writers use to claim space. they update w_count when done.
    uint64 w_pending;
    ptr items;
    const meta* items_desc;
} ring;

TYPE_ID_DEF(ring, 30);

extern META_DECL(ring);

META_GETTER_FUNC(ring);

void ring_print(ring *self);

ring* ring_create(const meta *type, uint64 cap);
void ring_init(ring *self, const meta *type, uint64 cap);
void ring_deinit(ring *self);
void ring_destroy(ring *self);

bool ring_is_full(ring *self);
bool ring_is_empty(ring *self);

bool ring_write(ring *self, const meta *type, ptr item);
bool ring_read(ring *self, const meta *type, ptr item);

