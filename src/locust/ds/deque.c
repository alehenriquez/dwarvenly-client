#ifndef DEQUE_H
#include <ds/deque.h>
#endif

META_FIELDS_DEF(deque) = {
    4, {
        {TYPE_ID_OF(uint64), sizeof(uint64), "len"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "head"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "tail"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items_desc"},
    }
};

META_DEF(deque, & META_FIELDS_OF(deque), nil);

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ATOMIC_H
#include <sched/atomic.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

deque* deque_create(const meta *type) {
    assert(type != nil);
    deque *d = mem_get(& META_OF(deque), 1);
    assert(d != nil);
    deque_init(d, type);
    return d;
}

void deque_init(deque *self, const meta *type) {
    assert(self != nil);
    assert(type != nil);
    self->items_desc = type;
    self->len = 0;

    // Set up a token object to always have at least 1 item in deque.
    // Allows us to not worry about null pointer head and tail ever.
    // Good for parallelism.
    self->head = self->tail = mem_get(sizeof(deque_node)+type->size);
    assert(self->head != nil);
    mem_set(self->head, sizeof(deque_node)+type->size, 0);
}

void deque_deinit(deque *self) {
    assert(self != nil);
    deque_node* curr = self->head;
    if (curr == nil) {
        return;
    }
    deque_node* next = curr->next;
    while (curr != nil) {
        mem_free(curr);
        curr = next;
        next = curr->next;
    }
}

void deque_destroy(deque* self) {
    assert(self != nil);
    deque_deinit(self);
    mem_free(self);
}

void deque_back_push(deque *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    deque_node *n = mem_get(sizeof(deque_node)+type->size);
    assert(n != nil);

    mem_copy(item, &n->content, type->size);
    n->next = nil;

	self->tail->next = n;
	self->tail = n;
    ++self->len;
}

void deque_front_push(deque *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    deque_node *n = mem_get(sizeof(deque_node)+type->size);
    assert(n != nil);

    mem_copy(item, &n->content, type->size);
    n->next = nil;

    // the ->next Keeps the token object at the real head of the deque
    deque_node *head = atomic_swap(self->head->next, n);
    if (head) {
        n->next = head;
    }
    ++self->len;
}

bool deque_front_pop(deque *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    deque_node* next = self->head->next;
	
    if (next) {
        self->head->next = next->next;
		if ((uintptr_t)self->tail == (uintptr_t)next) {
			self->tail = self->head;
		}
        mem_copy(&next->content, item, type->size);
        --self->len;
        mem_free(next);
        return true;
    }

    return false;
}

bool deque_back_pop(deque *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    deque_node* tail = self->tail;
    deque_node* next = tail->next;

    if (next) {
        self->tail = next;
        mem_copy(&next->content, item, type->size);
        --self->len;
        mem_free(tail);
        return true;
    }

    return false;
}


