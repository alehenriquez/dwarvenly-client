#define TYPES_H

#include <stdint.h>
#include <stddef.h>
/*
 * basic types
 */
typedef uint8_t  byte;
#ifndef _WIN32
typedef uint8_t bool;
#endif
typedef int8_t   int8;
typedef uint8_t  uint8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef int32_t  int32;
typedef uint32_t uint32;
typedef int64_t  int64;
typedef uint64_t uint64;
typedef float  float32;
typedef double float64;
typedef void* ptr; // The only pointer type! \o/
typedef int32 type_id;

// stdbool has true and false as 1 and 0 macros
#undef true
#define true 1
#undef false
#define false 0

/*
 * get rid of C types
 * the / / / forces a syntax error immediately,
 * which will show "last name: XXunsigned".
 */
//#define unsigned XXunsigned / / /
//#define signed XXsigned / / /
//#define char XXchar / / /
//#define short XXshort / / /
//#define int XXint / / /
//#define long XXlong / / /
//#define float XXfloat / / /
//#define double XXdouble / / /
//#define size_t XXsize_t / / /

// Concatenate preprocessor tokens A and B without expanding macro definitions
// (however, if invoked from a macro, macro arguments are expanded).
#define __CONCAT_(A, B) A ## B

// Concatenate preprocessor tokens A and B after macro-expanding them.
#undef __CONCAT
#define __CONCAT(A, B) __CONCAT_(A, B)
//Then, e.g., __CONCAT(s, 1) produces the identifier s1.

// Turn A into a string literal without expanding macro definitions
// (however, if invoked from a macro, macro arguments are expanded).
#define __STRINGIFY_(A) #A

// Turn A into a string literal after macro-expanding it.
#define __STRINGIFY(A) __STRINGIFY_(A)
// __STRINGIFY(__CONCAT(s, 1)) // produces "s1"

#define TYPE_ID_OF(type) __CONCAT(type, __type_id)
#define TYPE_ID_DEF(name, id) static const type_id TYPE_ID_OF(name) = id

// unique type identifiers
TYPE_ID_DEF(type_id, 0);
TYPE_ID_DEF(bool, 1);
TYPE_ID_DEF(byte, 2);
TYPE_ID_DEF(int8, 3);
TYPE_ID_DEF(uint8, 4);
TYPE_ID_DEF(int16, 5);
TYPE_ID_DEF(uint16, 6);
TYPE_ID_DEF(int32, 7);
TYPE_ID_DEF(uint32, 8);
TYPE_ID_DEF(int64, 9);
TYPE_ID_DEF(uint64, 10);
TYPE_ID_DEF(float32, 13);
TYPE_ID_DEF(float64, 14);
TYPE_ID_DEF(ptr, 15);

// Negative values denote C types that are not Locust types.
TYPE_ID_DEF(hidden, -1);
TYPE_ID_DEF(size_t, -2);
TYPE_ID_DEF(uintptr_t, -3);
TYPE_ID_DEF(ptrdiff_t, -4);

// Helper macros
#ifdef _WIN32
#define overloaded 
#else
#define overloaded __attribute__((overloadable))
#endif

#define unreachable __builtin_unreachable();
#define nil ((ptr)(0))

#define __VA_NARGS__(...) __PP_NARG_(__VA_ARGS__, __PP_RSEQ_N())
#define __PP_NARG_(...)   __PP_ARG_N(__VA_ARGS__)
#define __PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define __PP_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0


// Meta information for types
typedef struct {
    uint16 len;
    type_id args[];
} args_list;

typedef struct {
    const args_list* takes;
    const args_list* returns;
    ptr name;
} method_info;

typedef struct {
    type_id type;
    size_t size;
    ptr name;
} field_info;

typedef struct {
    uint16 len;
    method_info methods[];
} methods_list;

typedef struct {
    uint16 len;
    field_info fields[];
} fields_list;

typedef struct {
    type_id type;
    size_t size;
    ptr name;
    const fields_list *fields;
    const methods_list *methods;
} meta;

TYPE_ID_DEF(meta, 18);

#define META_FIELDS_OF(type) __CONCAT(type, __meta_fields)
#define META_FIELDS_DEF(type) static const fields_list META_FIELDS_OF(type)
#define META_METHODS_OF(type) __CONCAT(type, __meta_methods)
#define META_METHODS_DEF(type) static const methods_list META_METHODS_OF(type)
#define META_OF(type) __CONCAT(type, __meta)
#define META_DECL(typ) const meta META_OF(typ)
#define META_DEF(typ, fields_ptr, methods_ptr) META_DECL(typ) = {TYPE_ID_OF(typ), sizeof(typ), __STRINGIFY(typ), (const fields_list*)fields_ptr, (const methods_list*)methods_ptr}
#define META_DEF_SIMPLE(type) META_DEF(type, nil, nil)
// Meta Descriptions of the base types

extern META_DECL(bool);
extern META_DECL(byte);
extern META_DECL(int8);
extern META_DECL(uint8);
extern META_DECL(int16);
extern META_DECL(uint16);
extern META_DECL(int32);
extern META_DECL(uint32);
extern META_DECL(int64);
extern META_DECL(uint64);
extern META_DECL(float32);
extern META_DECL(float64);
extern META_DECL(ptr);

extern META_DECL(type_id);

// C (not Locust) types
extern META_DECL(size_t);
extern META_DECL(uintptr_t);
extern META_DECL(ptrdiff_t);

extern META_DECL(meta);

// Description Getters for base types
#define META_GETTER_FUNC(type) static inline const meta * overloaded describe(type *_) { return & META_OF(type); }

META_GETTER_FUNC(meta);
META_GETTER_FUNC(int8);
META_GETTER_FUNC(uint8);
META_GETTER_FUNC(int16);
META_GETTER_FUNC(uint16);
META_GETTER_FUNC(int32);
META_GETTER_FUNC(uint32);
META_GETTER_FUNC(int64);
META_GETTER_FUNC(uint64);
META_GETTER_FUNC(float32);
META_GETTER_FUNC(float64);
static inline const meta * overloaded describe(void *_) { return & META_OF(ptr); }


// Description Inquisitors
void repr(ptr item, const meta *m);

static inline bool types_equal_size(const meta *a, const meta *b) {
    return (a->size == b->size);
}

static inline bool types_are_equal(const meta *a, const meta *b) {
    return (a->type == b->type);
}

//#define strings_match(str1, str2) while(str1 == str2

/*
static inline bool type_has_field(const meta *tm, const field_info *fm) {
    if (tm->fields == 0) {
        return false;
    }
    uint16 i = 0;
    uint16 len = tm->fields->len;
    field_info cmp = *fm;
    field_info curr;
    ptr a, b;
    byte a_, b_;
    while (i < len) {
        curr = tm->fields->fields[i];
        if (curr.type != cmp.type) { return false; }
        if (curr.size != cmp.size) { return false; }
        a = curr.name;
        b = cmp.name;
        do {
            a_ = *(((byte*)a)++);
            b_ = *(((byte*)b)++);
            if (a_ == 0 && b_ == 0) { return true; }
        } while (a_ == b_);

        return false;
        ++i;
    }
    return false;
}
*/


#define TYPE_CHECK(td1, td2) assert(types_are_equal(td1, td2))













