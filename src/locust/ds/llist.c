#ifndef LLIST_H
#include <ds/llist.h>
#endif

typedef struct llist_node {
    struct llist_node *next;
    byte data[];
} llist_node;

META_FIELDS_DEF(llist) = {
    4, {
        {TYPE_ID_OF(ptr), sizeof(ptr), "head"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "tail"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "len"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items_desc"},
    }
};

META_DEF(llist, & META_FIELDS_OF(llist), nil);

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#ifndef ATOMIC_H
#include <sched/atomic.h>
#endif

#include <stdio.h>
#include <stdlib.h>

llist* llist_create(const meta *type) {
    assert(type != nil);
    llist *self = mem_get(& META_OF(llist), 1);
    assert(self != nil);
    llist_init(self, type);
    return self;
}

void llist_init(llist *self, const meta *type) {
    assert(self != nil);
    assert(type != nil);
    self->len = 0;
    self->head = nil;
    self->tail = nil;
    self->items_desc = type;
}

void llist_deinit(llist *self) {
    assert(self != nil);
    const meta *type = self->items_desc;
    byte tmp[type->size];
    while (llist_front_pop(self, type, &tmp));
}

void llist_destroy(llist *self) {
    assert(self != nil);
    llist_deinit(self);
    mem_free(self);
}

bool llist_front_pop(llist *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    if (self->head == nil) {
        return false;
    }

    // obtain data
    llist_node *curr = (llist_node*)self->head;
    mem_copy(&curr->data, item, type->size);

    // alter container
	if (self->head == self->tail) {
		// need to alter tail as well.
		atomic_store((uint128*)&self->head, 0);
	} else {
    	self->head = (ptr)((llist_node*)self->head)->next;
	}
    --self->len;

    // cleanup
    mem_free(curr);
    return true;
}

bool llist_back_pop(llist *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    if (self->tail == nil) {
        return false;
    }

    // obtain data
    llist_node *curr = (llist_node*)self->tail;
    mem_copy(&curr->data, item, type->size);

    // alter container
	if (self->head == self->tail) {
		// need to alter tail as well.
		atomic_store((uint128*)&self->head, 0);
	} else {
	    self->head = (ptr)((llist_node*)self->head)->next;
	}
    --self->len;

    // cleanup
    mem_free(curr);
    return true;
}

void llist_back_push(llist *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    llist_node *node = mem_get(sizeof(llist_node)+type->size);
    assert(node != nil);
    mem_copy(item, &node->data, type->size);
    node->next = nil;

    if (self->tail != nil) {
        ((llist_node*)self->tail)->next = node;
    } else {
        self->head = (ptr)node;
    }
    self->tail = (ptr)node;
    ++self->len;
}

void llist_front_push(llist *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    llist_node *node = mem_get(sizeof(llist_node)+type->size);
    assert(node != nil);
    mem_copy(item, &node->data, type->size);

    if (self->head != nil) {
        node->next = (llist_node*)self->head;
    } else {
        node->next = (llist_node*)nil;
        self->tail = (ptr)node;
    }
    self->head = (ptr)node;
    ++self->len;
}

