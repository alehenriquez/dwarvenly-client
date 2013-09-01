#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif


void var_set(var *self, meta const * const type, ptr data) {
    error_goto_if(self == nil, "NilPtr: self");
    error_goto_if(type == nil, "NilPtr: type");
    error_goto_if(data == nil, "NilPtr: data");
    error_goto_if(self->type->type != type->type, "TypeMismatch: self: %s, new: %s", self->type->name, type->name);
    self->type = type;
    mem_copy(data, self->data, type->size);
  error:
    return;
}
