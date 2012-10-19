#define VAR_H

#ifndef TYPES_H
#include <types/types.h>
#endif

#ifndef MEM_H
#include <rt/mem.h>
#endif

typedef struct {
    const meta *type;
    ptr data;
} var;

TYPE_ID_DEF(var, 16);

extern META_DECL(var);

META_GETTER_FUNC(var);

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#define var_type_check(var, type) TYPE_CHECK(var->type, type)

inline void var_init(var *self, const meta *type, ptr data) {
    assert(self != nil);
    assert(type != nil);
    assert(data != nil);
    self->type = type;
    mem_copy(data, &self->data, type->size);
}

inline var* var_create(const meta *type, ptr data) {
    assert(type != nil);
	assert(data != nil);
    var *v = mem_get(sizeof(var)+type->size);
    assert(v != nil);
	var_init(v, type, data);
    return v;
}

inline void var_deinit(var *self) {
    assert(self != nil);

}

inline void var_destroy(var *self) {
    assert(self != nil);
    var_deinit(self);
	mem_free(self);
}

inline void var_set(var *self, const meta *type, ptr data) {
    assert(self != nil);
    assert(type != nil);
    assert(data != nil);
    mem_copy(data, &self->data, self->type->size);
}

inline void var_get(var *self, const meta *type, ptr data) {
    assert(self != nil);
    assert(type != nil);
    assert(data != nil);
    mem_copy(&self->data, data, self->type->size);
}

