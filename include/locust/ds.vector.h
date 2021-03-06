#define VECTOR_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

// Vector is a lot like C++'s vector in costs. The differences are:
// - insertion is only allowed at the end. (you can still alter values anywhere)
// - atomicity guarantees. :) (which is why the previous happened.)

typedef struct {
    uint64 len;
    uint64 cap;
    ptr items;
    const meta* items_desc;
} vector;

#define vector__type_id ((type_id)28)
extern TYPE_ID_DECL(vector);
extern META_DECL(vector);

void overloaded vector_init(vector *self, const meta *type);
void overloaded vector_init(vector *self, const meta *type, uint64 reserved);
void vector_deinit(vector *self);

bool overloaded nonnullfunc vector_grow(vector *self);
bool overloaded nonnullfunc vector_grow(vector *self, uint64 count);

bool vector_push(vector *self, const meta *type, ptr item);
bool vector_pop(vector *self, const meta *type, ptr item);
bool vector_set(vector *self, uint64 index, const meta *type, ptr item);
bool vector_get(vector *self, uint64 index, const meta *type, ptr item);

