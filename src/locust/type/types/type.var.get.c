#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif


void var_get(var *self, meta const * const type, ptr data) {
    error_goto_if(self == nil, "NilPtr: self");
    error_goto_if(type == nil, "NilPtr: type");
    error_goto_if(data == nil, "NilPtr: data");
    error_goto_if(!types_are_equal(self->type, type), "TypeMismatch: self->type %s != type %s", self->type->name, type->name);
    mem_copy(self->data, data, type->size);
  error:
    return;
}
