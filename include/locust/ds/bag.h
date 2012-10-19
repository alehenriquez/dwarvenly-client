#define BAG_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct {
    uint64 len;
    uint64 cap;
    ptr items;
    const meta* items_desc;
} bag;

TYPE_ID_DEF(bag, 36);

extern META_DECL(bag);

META_GETTER_FUNC(bag);


bag* bag_create(const meta* type);
void bag_init(bag *self, const meta* type);
void bag_deinit(bag *self);
void bag_destroy(bag *self);

bool bag_insert(bag *self, const meta *type, ptr item);
bool bag_get(bag *self, uint64 index, const meta *type, ptr item);
bool bag_remove(bag *self, uint64 index);
