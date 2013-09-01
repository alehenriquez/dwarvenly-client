#define BAG_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

typedef struct {
    uint64 len;
    uint64 cap;
    ptr items;
    const meta* items_desc;
} bag;

#define bag__type_id ((type_id)36)
extern TYPE_ID_DECL(bag);
extern META_DECL(bag);

void bag_init(bag *self, const meta* type);
void bag_deinit(bag *self);

bool bag_insert(bag *self, const meta *type, ptr item);
bool bag_get(bag *self, uint64 index, const meta *type, ptr item);
bool bag_remove(bag *self, uint64 index);
