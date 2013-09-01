#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

void slice_init(slice *self, meta const * const type, ptr object, uint32 start, uint32 stride, uint64 count) {
	error_goto_if(self == nil, "NilPtr: self");
    error_goto_if(type == nil, "NilPtr: type");
    error_goto_if(object == nil, "NilPtr: object");
    self->type = type;
    self->object = object;
    self->start = start;
    self->stride = stride;
    self->count = count;
    return;
  error:
  	self->type = nil;
  	self->object = nil;
  	self->start = 0;
  	self->stride = 0;
  	self->count = 0;
    return;
}