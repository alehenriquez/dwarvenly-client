#define ATOMIC_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

// Modified from libOPA
// https://trac.mcs.anl.gov/projects/openpa/
// Under an MIT license

// The base set of functions is
//   atomic_load(X *) => X
//   atomic_store(X *, X)
//   atomic_add(X *, X)
//   atomic_incr(X *)
//   atomic_decr(X *)
//   atomic_decr_and_test(X *) => bool
//   atomic_fetch_and_add(X *ptr, X) => X (old)
//   atomic_cas(X *ptr, X (old), X (new)) => X (value read)
//   atomic_swap(X *ptr, X (new)) => X (old)
// Where X is a supported atomic type that was declared as volatile
// Also supported are memory barriers via
//   atomic_r_barrier()
//   atomic_w_barrier()
//   atomic_rw_barrier()
//   atomic_wr_barrier()
// Where the last two are exactly the same, but just cover programmer memory lapse. :]
//
// So far this only supports x86/x64
// We could do a separate atomic.c for other platform support, as long as
// the barriers here are unmacro'd accordingly


// Modified from libOPA
// https://trac.mcs.anl.gov/projects/openpa/
// Under an MIT license

// Aligned loads and stores are atomic on x86(-64).
static inline int8   overloaded atomic_load(const int8 *pointer)   { return *pointer; }
static inline uint8  overloaded atomic_load(const uint8 *pointer)  { return *pointer; }
static inline int16  overloaded atomic_load(const int16 *pointer)  { return *pointer; }
static inline uint16 overloaded atomic_load(const uint16 *pointer) { return *pointer; }
static inline int32  overloaded atomic_load(const int32 *pointer)  { return *pointer; }
static inline uint32 overloaded atomic_load(const uint32 *pointer) { return *pointer; }
static inline int64   overloaded atomic_load(const int64 *pointer)   { return *pointer; }
static inline uint64  overloaded atomic_load(const uint64 *pointer)  { return *pointer; }
//static inline int128  overloaded atomic_load(const int128 *pointer)  { return *pointer; }
//static inline uint128 overloaded atomic_load(const uint128 *pointer) { return *pointer; }

// Aligned loads and stores are atomic on x86(-64).
static inline void overloaded atomic_store(int8 *pointer,     int8 val) { *pointer = val; }
static inline void overloaded atomic_store(uint8 *pointer,   uint8 val) { *pointer = val; }
static inline void overloaded atomic_store(int16 *pointer,   int16 val) { *pointer = val; }
static inline void overloaded atomic_store(uint16 *pointer, uint16 val) { *pointer = val; }
static inline void overloaded atomic_store(int32 *pointer,   int32 val) { *pointer = val; }
static inline void overloaded atomic_store(uint32 *pointer, uint32 val) { *pointer = val; }
static inline void overloaded atomic_store(int64 *pointer,     int64 val) { *pointer = val; }
static inline void overloaded atomic_store(uint64 *pointer,   uint64 val) { *pointer = val; }
//static inline void overloaded atomic_store(int128 *pointer,   int128 val) { *pointer = val; }
//static inline void overloaded atomic_store(uint128 *pointer, uint128 val) { *pointer = val; }


// atomic adds
static inline void overloaded atomic_add(int8 *pointer, int8 val) {
    __asm__ __volatile__ ("lock; addb %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(uint8 *pointer, uint8 val) {
    __asm__ __volatile__ ("lock; addb %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(int16 *pointer, int16 val) {
    __asm__ __volatile__ ("lock; addw %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(uint16 *pointer, uint16 val) {
    __asm__ __volatile__ ("lock; addw %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(int32 *pointer, int32 val) {
    __asm__ __volatile__ ("lock; addl %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(uint32 *pointer, uint32 val) {
    __asm__ __volatile__ ("lock; addl %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(int64 *pointer, int64 val) {
    __asm__ __volatile__ ("lock; addq %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}
static inline void overloaded atomic_add(uint64 *pointer, uint64 val) {
    __asm__ __volatile__ ("lock; addq %1,%0"    :"=m" (*pointer)  :"ir" (val), "m" (*pointer)); return;
}

// atomic increments
static inline void overloaded atomic_incr(int8 *pointer) {
    __asm__ __volatile__ ("lock; incb %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(uint8 *pointer) {
    __asm__ __volatile__ ("lock; incb %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(int16 *pointer) {
    __asm__ __volatile__ ("lock; incw %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(uint16 *pointer) {
    __asm__ __volatile__ ("lock; incw %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(int32 *pointer) {
    __asm__ __volatile__ ("lock; incl %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(uint32 *pointer) {
    __asm__ __volatile__ ("lock; incl %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(int64 *pointer) {
    __asm__ __volatile__ ("lock; incq %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_incr(uint64 *pointer) {
    __asm__ __volatile__ ("lock; incq %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}

// atomic decrements
static inline void overloaded atomic_decr(int8 *pointer) {
    __asm__ __volatile__ ("lock; decb %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(uint8 *pointer) {
    __asm__ __volatile__ ("lock; decb %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(int16 *pointer) {
    __asm__ __volatile__ ("lock; decw %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(uint16 *pointer) {
    __asm__ __volatile__ ("lock; decw %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(int32 *pointer) {
    __asm__ __volatile__ ("lock; decl %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(uint32 *pointer) {
    __asm__ __volatile__ ("lock; decl %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(int64 *pointer) {
    __asm__ __volatile__ ("lock; decq %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}
static inline void overloaded atomic_decr(uint64 *pointer) {
    __asm__ __volatile__ ("lock; decq %0"   :"=m" (*pointer)  :"m" (*pointer)); return;
}


// atomic decrement and test
static inline bool overloaded atomic_decr_and_test(int8 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decb %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(uint8 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decb %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(int16 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decw %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(uint16 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decw %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(int32 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decl %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(uint32 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decl %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(int64 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decq %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}
static inline bool overloaded atomic_decr_and_test(uint64 *pointer) {
    bool result; __asm__ __volatile__ ("lock; decq %0; setz %1":"=m" (*pointer), "=q" (result):"m" (*pointer)); return result;
}

// atomic fetch and add
static inline int atomic_fetch_and_add(int64* p, int64 val) {
    __asm__ __volatile__ ("lock; xadd %0,%2;" : "=r" (val) :  "0" (val),  "m" (*p) : "memory" ); return val;
}

// Compare And Swap
static inline int8 overloaded atomic_cmp_and_swap(int8 *pointer, int8 oldv, int8 newv) {
    int8 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline uint8 overloaded atomic_cmp_and_swap(uint8 *pointer, uint8 oldv, uint8 newv) {
    uint8 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline int16 overloaded atomic_cmp_and_swap(int16 *pointer, int16 oldv, int16 newv) {
    int16 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline uint16 overloaded atomic_cmp_and_swap(uint16 *pointer, uint16 oldv, uint16 newv) {
    uint16 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline int32 overloaded atomic_cmp_and_swap(int32 *pointer, int32 oldv, int32 newv) {
    int32 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline uint32 overloaded atomic_cmp_and_swap(uint32 *pointer, uint32 oldv, uint32 newv) {
    uint32 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline int64 overloaded atomic_cmp_and_swap(int64 *pointer, int64 oldv, int64 newv) {
    int64 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
static inline uint64 overloaded atomic_cmp_and_swap(uint64 *pointer, uint64 oldv, uint64 newv) {
    uint64 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return prev;
}
/*
static inline int128 overloaded atomic_cmp_and_swap(int128 *pointer, int128 oldv, int128 newv) {
  int128 prev;
  __asm__ __volatile__("movl (%3), %%rbx; movl 8(%3), %%rcx;"
                       "lock; cmpxchg16b (%1);"
                       : "=A" (prev)
                       : "D" (*pointer), "0" (oldv), "S" (&newv)
                       : "memory", "%rcx");
  return prev;
}
static inline uint128 overloaded atomic_cmp_and_swap(uint128 *pointer, uint128 oldv, uint128 newv) {
  uint128 prev;
  __asm__ __volatile__("movl (%3), %%rbx; movl 8(%3), %%rcx;"
                       "lock; cmpxchg16b (%1);"
                       : "=A" (prev)
                       : "D" (*pointer), "0" (oldv), "S" (&newv)
                       : "memory", "%rcx");
  return prev;
}
*/

static inline void* overloaded atomic_cmp_and_swap(ptr pointer, ptr oldv, ptr newv) {
    void *prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (pointer): "0" (oldv), "q" (newv), "m" (pointer)); return prev;
}


// Compare And Set
static inline bool overloaded atomic_cmp_and_set(int8 *pointer, int8 oldv, int8 newv) {
    int8 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(uint8 *pointer, uint8 oldv, uint8 newv) {
    uint8 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(int16 *pointer, int16 oldv, int16 newv) {
    int16 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(uint16 *pointer, uint16 oldv, uint16 newv) {
    uint16 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(int32 *pointer, int32 oldv, int32 newv) {
    int32 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(uint32 *pointer, uint32 oldv, uint32 newv) {
    uint32 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(int64 *pointer, int64 oldv, int64 newv) {
    int64 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(uint64 *pointer, uint64 oldv, uint64 newv) {
    uint64 prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (*pointer): "0" (oldv), "q" (newv), "m" (*pointer)); return (prev == oldv);
}
/*
static inline bool overloaded atomic_cmp_and_set(int128 *pointer, int128 oldv, int128 newv) {
  int128 prev;
  __asm__ __volatile__("movl (%3), %%rbx; movl 8(%3), %%rcx;"
                       "lock; cmpxchg16b (%1);"
                       : "=A" (prev)
                       : "D" (*pointer), "0" (oldv), "S" (&newv)
                       : "memory", "%rcx");
  return (prev == oldv);
}
static inline bool overloaded atomic_cmp_and_set(uint128 *pointer, uint128 oldv, uint128 newv) {
  uint128 prev;
  __asm__ __volatile__("movl (%3), %%rbx; movl 8(%3), %%rcx;"
                       "lock; cmpxchg16b (%1);"
                       : "=A" (prev)
                       : "D" (*pointer), "0" (oldv), "S" (&newv)
                       : "memory", "%rcx");
  return (prev == oldv);
}
*/

static inline bool overloaded atomic_cmp_and_set(ptr pointer, ptr oldv, ptr newv) {
    void *prev; __asm__ __volatile__ ("lock; cmpxchg %3,%4": "=a" (prev), "=m" (pointer): "0" (oldv), "q" (newv), "m" (pointer)); return (prev == oldv);
}


// Swaps
static inline int8 overloaded atomic_swap(int8 *pointer, int8 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline uint8 overloaded atomic_swap(uint8 *pointer, uint8 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline int16 overloaded atomic_swap(int16 *pointer, int16 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline uint16 overloaded atomic_swap(uint16 *pointer, uint16 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline int32 overloaded atomic_swap(int32 *pointer, int32 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline uint32 overloaded atomic_swap(uint32 *pointer, uint32 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline int64 overloaded atomic_swap(int64 *pointer, int64 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline uint64 overloaded atomic_swap(uint64 *pointer, uint64 val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (*pointer): "0" (val),  "m" (*pointer)); return val;
}
static inline void* overloaded atomic_swap(ptr pointer, ptr val) {
    __asm__ __volatile__ ("xchg %0,%1":"=r" (val), "=m" (pointer): "0" (val),  "m" (pointer)); return val;
}

// Barriers
#define atomic_w_barrier()  __asm__ __volatile__  ( "sfence" ::: "memory" )
#define atomic_r_barrier()  __asm__ __volatile__  ( "lfence" ::: "memory" )
#define atomic_rw_barrier() __asm__ __volatile__  ( "mfence" ::: "memory" )
#define atomic_wr_barrier() __asm__ __volatile__  ( "mfence" ::: "memory" )
