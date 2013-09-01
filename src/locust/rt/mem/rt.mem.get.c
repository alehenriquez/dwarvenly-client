#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#include <stdlib.h>

ptr
overloaded nonnullfunc mustuseretfunc mallocfunc mallocsizefunc(1)
mem_get(uint64 len, meta const * const m) {
	ptr p = malloc(len);
    error_goto_if(p == nil, "OutOfMemory, malloc() failed");
	return p;
  error:
    return p;
}