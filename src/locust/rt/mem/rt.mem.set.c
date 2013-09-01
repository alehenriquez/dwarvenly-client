#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif



#define __dupe_u8_to_u64(val) ((val)*0x0101010101010101ull)
#define __dupe_u16_to_u64(val) ((val)*0x0001000100010001ull)
#define __dupe_u32_to_u64(val) ((val)*0x0000000100000001ull)

void 
static inline nonnullfunc
__mem_set_u8_simple(ptr dest, uint64 len, uint8 val) {
    __asm__ __volatile__("cld; rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val), "c" (len)
        :);
}

void 
static inline nonnullfunc
__mem_set_u16_simple(ptr dest, uint64 len, uint16 val) {
    __asm__ __volatile__("cld; rep; stosw"
        :"=D" (dest)
        :"0" (dest), "a" (val), "c" (len & (~0x1ull))
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val>>8), "c" (len & 0x1)
        :);
}

void
static inline nonnullfunc
__mem_set_u32_simple(ptr dest, uint64 len, uint32 val) {
    __asm__ __volatile__("cld; rep; stosl"
        :"=D" (dest)
        :"0" (dest), "a" (val), "c" (len & (~0x3ull))
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 24), "c" ((len & 0x3) > 0)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 16), "c" ((len & 0x3) > 1)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 8), "c" ((len & 0x3) > 2)
        :);
}

void 
static inline nonnullfunc
__mem_set_u64_simple(ptr dest, uint64 len, uint64 val) {
    __asm__ __volatile__("cld; rep; stosq"
        :"=D" (dest)
        :"0" (dest), "a" (val), "c" (len & (~0x7ull))
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 56), "c" ((len & 0x7) > 0)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 48), "c" ((len & 0x7) > 1)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 40), "c" ((len & 0x7) > 2)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 32), "c" ((len & 0x7) > 3)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 24), "c" ((len & 0x7) > 4)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 16), "c" ((len & 0x7) > 5)
        :);
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest)
        :"0" (dest), "a" (val >> 8), "c" ((len & 0x7) > 6)
        :);
}

/*
static inline void __mem_set_u8(ptr dest, uint64 len, uint8 val) {
    // PLAT: x86_64
    // Naive memset
    uint8 offset_count;
    __asm__ __volatile__("andb $7, %%rax;"
        :"=A" (offset_count),
        :"0" (dest)
        :);
    // Set bytes until aligned 64 bits
    __asm__ __volatile__("cld; rep; stosb"
        :"=D" (dest),
        :"0" (dest), "a" (val), "c" (offset_count)
        :);
    // Set 64 bit sized at a time
    __asm__ __volatile__("rep; stosq"
        :"=D" (dest),
        :"0" (dest), "A" (__dupe_u8_to_u64(val)), "c" (len >> 3)
        :);
    // Finish out remainder as bytes
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest), 
        :"0" (dest), "a" (val), "c" (0x8 - (offset_count))
        :);
}

static inline void __mem_set_u16(ptr dest, uint64 len, uint16 val) {
    __asm__ __volatile__("cld; rep; stosb"
        :"=D" (dest),
        :"0" (dest), "a" (val), "c" (len & 0x7)
        :);
    // Set 64 bit sized at a time
    __asm__ __volatile__("rep; stosq"
        :"=D" (dest),
        :"0" (dest), "A" (__dupe_u8_to_u64(val)), "c" (len >> 3)
        :);
    // Finish out remainder as bytes
    __asm__ __volatile__("rep; stosb"
        :"=D" (dest), 
        :"0" (dest), "a" (val), "c" (0x8 - (len & 0x7))
        :);
}
*/


void
overloaded nonnullfunc
mem_set_(ptr dest, uint64 len, var val, char const * const func_name, char const * const file_name, int line) {
    switch(val.type->size) {
        case 8:
            __mem_set_u8_simple(dest, len, *((uint8*)val.data));
            break;
        case 16:
            __mem_set_u16_simple(dest, len, *((uint16*)val.data));
            break;
        case 32:
            __mem_set_u32_simple(dest, len, *((uint32*)val.data));
            break;
        case 64:
            __mem_set_u64_simple(dest, len, *((uint64*)val.data));
            break;
        default:
            error("NotImplemented: mem_set with as yet unsupported type size. Sorry.");
            break;
    }
}

/*
void mem_set(slice s, var value) {
    byte *dest = s.bytes;
    assert(dest != nil);
    
    uint64 src_size, src_size_iter;
    src_size = src_size_iter = value.meta.size;
    assert(src_size != 0);
    
    while (s.len-->0) {
        *(dest++) = 
    }
}
*/

//HURRDURR
/*
void mem_set(slice s, byte value) {
	assert(s.type != nil);
    assert(s.bytes != nil);
    assert(s.len != 0);
    
    __asm__ __volatile__(
        // tiny lengths get tiny loops. :D
        "cmp $0x7, %[len];\n"
        "jl 7f;\n"

        // loop until cache aligned
        "0:\n"
        "   movq %[loc], %%rcx;\n"
        "   andq $0x7, %%rcx;\n"
        "   subq %%rcx, %[len];\n"
        "1:\n"
        "   jrcxz 2f;\n"
        "   decq %%rcx;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 1b;\n"


        // aligned cache looper
        "2:\n"
        // construct cache line sized source since we're sure we need it now
        "   movb %%dl, %%dh;\n"
        "   movw %%dx, %%cx;\n"
        "   shlq $16, %%rcx;\n"
        "   movw %%dx, %%cx;\n"
        "   movl %%ecx, %%edx;\n"
        "   shlq $32, %%rdx;\n"
        "   movl %%ecx, %%edx;\n"

        "   movq %[len], %%rcx;\n"
        "   shrq $4, %%rcx;\n"
        "3:\n"
        "   jrcxz 4f;\n"
        "   decq %%rcx;\n"
        "   movq %%rdx, (%[loc]);\n"
        "   addq $0x8, %[loc];\n"
        "   jmp 3b;\n"

        "4:\n"
        "   movq %[len], %%rcx;\n"
        "   andq $0x7, %%rcx;\n"
        "5:\n"
        "   jrcxz 9f;\n"
        "   decq %%rcx;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 5b;\n"

        "6:\n"
        "   movb %b[len], %%cl;\n"
        "7:\n"
        "   jrcxz 9f;\n"
        "   decb %%cl;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 7b;\n"

        "9:\n"
        :
        : [loc]"Q"(s.bytes), [len]"Q"(s.len), "d"(value)
        : "rcx"
    );
}
*/


/*
void mem_set(ptr location, uint64 len, uint64 value) {
    assert(location != nil);
    assert(len != 0);
    __asm__ __volatile__(
        // tiny lengths get tiny loops. :D
        "cmp $0x7, %[len];\n"
        "jl 7f;\n"

        // loop until cache aligned
        "0:\n"
        "   movq %[loc], %%rcx;\n"
        "   andq $0x7, %%rcx;\n"
        "   subq %%rcx, %[len];\n"
        "1:\n"
        "   jrcxz 2f;\n"
        "   decq %%rcx;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 1b;\n"


        // aligned cache looper
        "2:\n"
        // construct cache line sized source since we're sure we need it now
        "   movb %%dl, %%dh;\n"
        "   movw %%dx, %%cx;\n"
        "   shlq $16, %%rcx;\n"
        "   movw %%dx, %%cx;\n"
        "   movl %%ecx, %%edx;\n"
        "   shlq $32, %%rdx;\n"
        "   movl %%ecx, %%edx;\n"

        "   movq %[len], %%rcx;\n"
        "   shrq $4, %%rcx;\n"
        "3:\n"
        "   jrcxz 4f;\n"
        "   decq %%rcx;\n"
        "   movq %%rdx, (%[loc]);\n"
        "   addq $0x8, %[loc];\n"
        "   jmp 3b;\n"

        "4:\n"
        "   movq %[len], %%rcx;\n"
        "   andq $0x7, %%rcx;\n"
        "5:\n"
        "   jrcxz 9f;\n"
        "   decq %%rcx;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 5b;\n"

        "6:\n"
        "   movb %b[len], %%cl;\n"
        "7:\n"
        "   jrcxz 9f;\n"
        "   decb %%cl;\n"
        "   movb %%dl, (%[loc]);\n"
        "   incq %[loc];\n"
        "   jmp 7b;\n"

        "9:\n"
        :
        : [loc]"Q"(location), [len]"Q"(len), "d"(value)
        : "rcx"
    );
}
*/


