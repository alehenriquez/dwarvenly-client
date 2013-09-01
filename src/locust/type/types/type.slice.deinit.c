#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif


void slice_deinit(slice *self) {
    error_goto_if(self == nil, "NilPtr: self");
    mem_set(self, sizeof(slice), 0);
  error:
  	return;
}
