#define ARRAY_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct {
    uint64 len;
    ptr bytes;
} array;

TYPE_ID_DEF(array, 27);

extern META_DECL(array);

META_GETTER_FUNC(array);

array* array_create(uint64 len);
void array_init(array *self, uint64 len);
void array_deinit(array *self);
void array_destroy(array *self);
