#define LLIST_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct {
    ptr *head;
    ptr *tail;
    uint64 len;
    const meta *items_desc;
} llist;

TYPE_ID_DEF(llist, 32);

extern META_DECL(llist);

META_GETTER_FUNC(llist);

llist* llist_create(const meta *type);
void llist_init(llist *self, const meta *type);
void llist_deinit(llist *self);
void llist_destroy(llist *self);

bool llist_front_pop(llist *self, const meta *type, ptr item);
void llist_front_push(llist *self, const meta *type, ptr item);
bool llist_back_pop(llist *self, const meta *type, ptr item);
void llist_back_push(llist *self, const meta *type, ptr item);
