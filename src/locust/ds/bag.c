#ifndef BAG_H
#include <ds/bag.h>
#endif

META_FIELDS_DEF(bag) = {
    4, {
        {TYPE_ID_OF(uint64), sizeof(uint64), "len"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "cap"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items_desc"},
    }
};

META_DEF(bag, & META_FIELDS_OF(bag), nil);

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

#define BLOCK_SIZE 8
#define BLOCK_INITIAL 8

bag* bag_create(const meta* type) {
    assert(type != nil);
    bag *self = mem_get(& META_OF(bag), 1);
    assert(self != nil);
    bag_init(self, type);
    return self;
}

void bag_init(bag *self, const meta* type) {
    assert(self != nil);
    assert(type != nil);
    self->items_desc = type;
    self->items = mem_get(type, BLOCK_INITIAL);
    assert(self->items != nil);
    self->len = 0;
    self->cap = BLOCK_INITIAL;
    return;
}

void bag_deinit(bag *self) {
    assert(self != nil);
    mem_free(self->items);
}

void bag_destroy(bag *self) {
    assert(self != nil);
    bag_deinit(self);
    mem_free(self);
}

bool bag_insert(bag *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);

    // Check for needing to enlarge
    if (self->len == self->cap) {
        ptr t = mem_resize(self->items, self->items_desc, self->cap+BLOCK_SIZE);
        if (t == NULL) {
            fprintf(stderr, "bag_insert() error: resizing failed\n");
            return false;
        }
        self->items = t;
        self->cap += BLOCK_SIZE;
    }

    // Add item
    uint64 index = atomic_fetch_and_add(&self->len, (uint64)1);
    uint64 size = self->items_desc->size;
    mem_copy(item, self->items+(index*size), size);
    return true;
}

bool bag_get(bag *self, uint64 index, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);

    // Bounds check
    if (index >= self->len) {
        return false;
    }
    uint64 size = self->items_desc->size;
    mem_copy(self->items+(index*size), item, size);
    return true;
}

bool bag_remove(bag *self, uint64 index) {
    assert(self != nil);
    // Bounds check
    if (index >= self->len) {
        return false;
    }

    ptr *items = self->items;
    ptr tmp = items[self->len-1];
    items[self->len-1] = items[index];
    items[index] = tmp;
    items[self->len-1] = NULL;
    --self->len;

    // Check for possible shrinking.
    if (((self->cap - self->len) > (BLOCK_SIZE * 2))
        && (self->cap > (BLOCK_INITIAL+BLOCK_SIZE))) {
        ptr t = mem_resize(self->items, self->items_desc, self->cap-BLOCK_SIZE);
        if (t == NULL) {
            fprintf(stderr, "bag_remove() error: resizing failed\n");
            return false;
        }
        self->items = t;
        self->cap -= BLOCK_SIZE;
    }
    return true;
}



