#ifndef VECTOR_H
#include <locust/ds.vector.h>
#endif

TYPE_ID_DEF(vector);

META_FIELDS_DEF(vector) = {
    4, {
        {TYPE_ID_OF(uint64), sizeof(uint64), "len"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "cap"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items_desc"},
    }
};

META_DEF(vector, & META_FIELDS_OF(vector), nil);

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef ATOMIC_H
#include <locust/sched.atomic.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8
#define BLOCK_INITIAL 8

void
overloaded nonnullfunc
vector_init(vector *self, meta const * const type) {
    self->len = 0;
    self->cap = 0;
    self->items_desc = type;
    self->items = nil;
    return;
}

void
overloaded nonnullfunc
vector_init(vector *self, meta const * const type, uint64 reserved) {
    self->len = 0;
    self->cap = type->size * reserved;
    self->items_desc = type;
    self->items = nil;

    if (reserved > 0) {
        self->items = mem_get(self->cap);
        error_if(self->items == nil, "vector_init() error: allocating failed with %llu of type %s\n", reserved, type->name);
    }
    return;
}

void nonnullfunc vector_deinit(vector *self) {
    mem_free(self->items);
}

bool overloaded nonnullfunc vector_grow(vector *self) {
    return vector_grow(self, 0);
}

bool overloaded nonnullfunc vector_grow(vector *self, uint64 count) {
    if (self->cap == 0 || self->items == nil) {
        // initial allocation
        if (count == 0) {
            count = BLOCK_INITIAL;
        }
        self->items = mem_get(count*self->items_desc->size, self->items_desc);
        error_goto_if(self->items == nil, "vector_grow() error: allocating failed\n");
    } else {
        // growing allocation
        if (count == 0) {
            count = BLOCK_SIZE;
        }
        ptr t = mem_resize(self->items, (self->cap+count)*self->items_desc->size, self->items_desc);
        error_goto_if(t == nil, "vector_grow() error: resizing failed\n");
        self->items = t;
    }
    self->cap += count;
    return true;
  error:
    return false;
}

bool nonnullfunc vector_set(vector *self, uint64 index, const meta *type, ptr item) {
    // Bounds check
    error_goto_if(index >= self->len, "vector_set() error: index out of bounds. %ld >= %ld.\n", index, self->len);

    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    // J-j-jam it in.
    mem_copy(item, items_bytes+(index*stride), stride);
    return true;
  error:
    return false;
}

bool nonnullfunc vector_push(vector *self, const meta *type, ptr item) {
    // Check for needing to allocate initial space or growing
    if (self->len == self->cap) {
        error_goto_if(vector_grow(self) == false, "vector_push() error: growing failed\n");
    }

    // Get our index into items
    uint64 index = atomic_fetch_and_add(&self->len, 1);
    // J-j-jam it in.
    mem_copy(item, self->items+(index*self->items_desc->size), self->items_desc->size);
    return true;
  error:
    return false;
}

bool nonnullfunc vector_get(vector *self, uint64 index, const meta *type, ptr item) {
    // Bounds check
    error_goto_if(index >= self->len, "vector_get() error: index out of bounds. %ld >= %ld.\n", index, self->len);

    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    mem_copy(items_bytes+(index*stride), item, stride);
    return true;
  error:
    return false;
}

bool nonnullfunc vector_pop(vector *self, const meta *type, ptr item) {
    if (self->len < 1) {
        return false;
    }

    uint64 index = atomic_fetch_and_add(&self->len, (int64)-1);
    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    mem_copy(items_bytes+((index-1)*stride), item, stride);

    // TODO: decide whether or not we shrink ever
    /*
    // Check for possible shrinking.
    if (((self->cap - self->len) > (BLOCK_SIZE * 2))
        && (self->cap > (BLOCK_INITIAL+BLOCK_SIZE))) {
        ptr t = mem_resize(self->items, self->cap-BLOCK_SIZE, self->items_desc);
        error_goto_if(t == nil, "vector_remove() error: resizing failed\n");
        self->items = t;
        self->cap -= BLOCK_SIZE;
    }
    */
    return true;
  error:
    return false;
}

