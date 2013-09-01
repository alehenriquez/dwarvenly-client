#ifndef TYPES_H
#include <locust/type.types.h>
#endif


#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif


void var_deinit(var *self) {
    error_goto_if(self == nil, "NilPtr: self");
    error_goto_if(self->data == nil, "NilPtr: self->data");
    error_goto_if(self->type == nil, "NilPtr: self->type");
    mem_set(self->data, self->type->size, 0);
  error:
  	return;
}
