#define DEQUE_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct deque_node {
    struct deque_node* next;
    byte content[];
} deque_node;

typedef struct {
    uint64  len;
    deque_node* head;
    deque_node* tail;
    const meta *items_desc;
} deque;

TYPE_ID_DEF(deque, 34);

extern META_DECL(deque);

META_GETTER_FUNC(deque);

deque* deque_create(const meta *type);
void deque_init(deque *self, const meta *type);
void deque_deinit(deque *self);
void deque_destroy(deque* self);

void deque_front_push(deque *self, const meta *type, ptr item);
bool deque_front_pop(deque* self, const meta *type, ptr item);

void deque_back_push(deque *self, const meta *type, ptr item);
bool deque_back_pop(deque* self, const meta *type, ptr item);

