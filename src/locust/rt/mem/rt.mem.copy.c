#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif


void 
static inline nonnullfunc
__mem_copy_fwd(ptr restrict from, ptr restrict to, uint64 len) {
    // PLAT: x86_64
    // Naive forward memcopy
    /* Copy bytes until aligned 64 bits */
    __asm__ __volatile__("cld;");
    __asm__ __volatile__("rep; movsb"
        :"=D" (to), "=S" (from)
        :"0" (to), "1" (from), "c" (len)
        :"memory");
}

void
static inline nonnullfunc
__mem_copy_bwd(ptr from, ptr to, uint64 len) {
    // PLAT: x86_64
    /* Copy bytes until aligned 64 bits */
    __asm__ __volatile__("std;");
    __asm__ __volatile__("rep; movsb"
        :"=D" (to), "=S" (from)
        :"0" (to+len), "1" (from+len), "c" (len)
        :"memory");
}

void 
nonnullfunc
mem_copy_(ptr from, ptr to, uint64 len, char const * const func_name, char const * const file_name, int line) {
    // PLAT: x86_64
    warn_goto_if(len == 0, "NoOp: from = %p, to = %p, len = 0", from, to);
    warn_goto_if((uintptr_t)from == (uintptr_t)to, "NoOp: from = %p, to = %p, len = %llu", from, to, len);

    // Core of memcpy, without extraneous checks for cases we can be sure of.
    if ((uintptr_t)from > (uintptr_t)to || (uintptr_t)from < ((uintptr_t)to-len)) {
        // No overlapping memory regions; Forward copy.
        __mem_copy_fwd(from, to, len);
    } else {
        // Overlapping memory regions; Copy backwards.
        __mem_copy_bwd(from, to, len);
    }

  error:
  warn:
    return;
}
