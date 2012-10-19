#ifndef VECTOR_H
#include <ds/vector.h>
#endif

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
#include <rt/mem.h>
#endif

#ifndef ATOMIC_H
#include <sched/atomic.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8
#define BLOCK_INITIAL 8

vector* vector_create(const meta *type) {
    assert(type != nil);
    vector *self = mem_get(& META_OF(vector), 1);
    assert(self != nil);
    vector_init(self, type);
    return self;
}

void overloaded vector_init(vector *self, const meta *type) {
    assert(self != nil);
    assert(type != nil);
    self->len = 0;
    self->cap = 0;
    self->items_desc = type;
    self->items = NULL;
    return;
}

void overloaded vector_init(vector *self, const meta *type, uint64 reserved) {
    assert(self != nil);
    assert(type != nil);
    self->items = NULL;
    self->len = 0;
    self->cap = reserved;
    self->items_desc = type;

    if (reserved == 0) {
        self->items = NULL;
    } else {
        self->items = mem_get(type, reserved);
        assert(self->items != nil);
    }
    return;
}

void vector_deinit(vector *self) {
    assert(self != nil);
    mem_free(self->items);
}

void vector_destroy(vector *self) {
    assert(self != nil);
    vector_deinit(self);
    mem_free(self);
}


bool vector_set(vector *self, uint64 index, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    // Bounds check
    if (index >= self->len) {
        fprintf(stderr, "vector_set() error: index out of bounds. %ld >= %ld ... ignored set.\n", index, self->len);
        return false;
    }

    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    // J-j-jam it in.
    mem_copy(item, items_bytes+(index*stride), stride);
    return true;
}

bool vector_push(vector *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);

    // Check for needing to allocate initial space
    if (self->cap == 0) {
        self->items = mem_get(self->items_desc, BLOCK_INITIAL);
        if (self->items == nil) {
            fprintf(stderr, "vector_set() error: allocating failed\n");
            return false;
        }
        self->cap = BLOCK_INITIAL;
    }

    // Check for needing to enlarge
    if (self->len == self->cap) {
        ptr t = mem_resize(self->items, self->items_desc, self->cap+BLOCK_SIZE);
        if (t == nil) {
            fprintf(stderr, "vector_set() error: resizing failed\n");
            return false;
        }
        self->items = t;
        self->cap += BLOCK_SIZE;
    }

    // Get our index into items
    uint64 index = atomic_fetch_and_add(&self->len, (uint64)1);

    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    // J-j-jam it in.
    mem_copy(item, items_bytes+(index*stride), stride);
    return true;
}

bool vector_get(vector *self, uint64 index, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    // Bounds check
    if (index >= self->len) {
        fprintf(stderr, "vector_get() error: index out of bounds. %ld >= %ld ... returning NULL.\n", index, self->len);
        return false;
    }

    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    mem_copy(items_bytes+(index*stride), item, stride);
    return true;
}

bool vector_pop(vector *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    if (self->len < 1) {
        return false;
    }

    uint64 index = atomic_fetch_and_add(&self->len, (int64)-1);
    uint32 stride = self->items_desc->size;
    byte* items_bytes = self->items;
    mem_copy(items_bytes+((index-1)*stride), item, stride);

    // Check for possible shrinking.
    if (((self->cap - self->len) > (BLOCK_SIZE * 2))
        && (self->cap > (BLOCK_INITIAL+BLOCK_SIZE))) {
        ptr t = mem_resize(self->items, self->items_desc, self->cap-BLOCK_SIZE);
        if (t == nil) {
            fprintf(stderr, "vector_remove() error: resizing failed\n");
            return false;
        }
        self->items = t;
        self->cap -= BLOCK_SIZE;
    }
    return true;
}

