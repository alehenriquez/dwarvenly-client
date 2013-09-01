#define DEQUE_H

#ifndef TYPES_H
#include <locust/type.types.h>
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

#define deque__type_id ((type_id)34)
extern TYPE_ID_DECL(deque);
extern META_DECL(deque);

void deque_init(deque *self, const meta *type);
void deque_deinit(deque *self);

void deque_front_push(deque *self, const meta *type, ptr item);
bool deque_front_pop(deque* self, const meta *type, ptr item);

void deque_back_push(deque *self, const meta *type, ptr item);
bool deque_back_pop(deque* self, const meta *type, ptr item);

