#define MAP_H

#ifndef TYPES_H
#include <types/types.h>
#endif

#ifndef HASH_H
#include <ds/hash.h>
#endif

typedef struct {
    ptr items;
    const meta *items_desc;
    uint32 len;
    uint32 cap;
    uint32 items_stride;
} map;

TYPE_ID_DEF(map, 31);

extern META_DECL(map);

META_GETTER_FUNC(map);

map* map_create(const meta *type);
inline void overloaded map_init(map *self, const meta *type);
void overloaded map_init(map *self, const meta *type, uint64 reserved);
void map_deinit(map *self);
void map_destroy(map *self);

bool map_set(map *self, uint32 key, const meta *type, ptr item);
bool map_get(map *self, uint32 key, const meta *type, ptr item);
bool map_pop(map *self, uint32 key, const meta *type, ptr item);
bool map_del(map *self, uint32 key);
bool map_has(map *self, uint32 key);




















