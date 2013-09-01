#define DLLIST_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

typedef struct {
    uint64 len;
    ptr head;
    ptr tail;
    const meta* items_desc;
} dllist;

#define dllist__type_id ((type_id)33)
extern TYPE_ID_DECL(dllist);
extern META_DECL(dllist);

void dllist_init(dllist *self, const meta *type);
void dllist_deinit(dllist *self);

uint64 dllist_len(dllist *self);
bool dllist_front_pop(dllist *self, const meta *type, ptr item);
void dllist_front_push(dllist *self, const meta *type, ptr item);
bool dllist_back_pop(dllist *self,const meta *type, ptr item);
void dllist_back_push(dllist *self, const meta *type, ptr item);

