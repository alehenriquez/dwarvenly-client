#define DLLIST_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct {
    uint64 len;
    ptr head;
    ptr tail;
    const meta* items_desc;
} dllist;

TYPE_ID_DEF(dllist, 33);

extern META_DECL(dllist);

META_GETTER_FUNC(dllist);

dllist* dllist_create(const meta *type);
void dllist_init(dllist *self, const meta *type);
void dllist_deinit(dllist *self);
void dllist_destroy(dllist *self);

uint64 dllist_len(dllist *self);
bool dllist_front_pop(dllist *self, const meta *type, ptr item);
void dllist_front_push(dllist *self, const meta *type, ptr item);
bool dllist_back_pop(dllist *self,const meta *type, ptr item);
void dllist_back_push(dllist *self, const meta *type, ptr item);

