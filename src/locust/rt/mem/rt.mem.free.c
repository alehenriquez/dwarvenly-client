#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#include <stdlib.h>

void
nonnullfunc
mem_free(ptr p) {
    free(p);
}