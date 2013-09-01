#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#include <stdlib.h>

ptr
overloaded nonnullfunc mustuseretfunc mallocsizefunc(2)
mem_resize(ptr p, uint64 len, meta const * const m) {
    error_goto_if((p = realloc(p, len)) == nil, "OutOfMemory, realloc() failed");
  error:
    return p;
}