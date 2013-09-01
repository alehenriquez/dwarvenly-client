#define TYPES_H

#include <stdint.h>
#include <stddef.h>
// basic types
typedef uint8_t  byte;
typedef uint8_t  bool;
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
typedef void* ptr;
typedef int32 type_id;

// stdbool has true and false as 1 and 0 macros
#undef true
#define true ((bool)1)
#undef false
#define false ((bool)0)

// Concatenate preprocessor tokens A and B without expanding macro definitions
// (however, if invoked from a macro, macro arguments are expanded).
#define CONCAT_(A, B) A ## B

// Concatenate preprocessor tokens A and B after macro-expanding them.
#undef CONCAT
#define CONCAT(A, B) CONCAT_(A, B)
//Then, e.g., __CONCAT(s, 1) produces the identifier s1.

// Turn A into a string literal without expanding macro definitions
// (however, if invoked from a macro, macro arguments are expanded).
#define STRINGIFY_(A) #A

// Turn A into a string literal after macro-expanding it.
#undef STRINGIFY
#define STRINGIFY(A) STRINGIFY_(A)
// ·STRINGIFY(·CONCAT(s, 1)) // produces "s1"

#define TYPE_ID_OF(type) CONCAT(type, __type_id)
#define TYPE_ID_LOC_OF(type) CONCAT(type, __type_id_loc)
#define TYPE_ID_DECL(name) const type_id TYPE_ID_LOC_OF(name)
#define TYPE_ID_DEF(name) TYPE_ID_DECL(name)  __attribute__(( type_tag_for_datatype(locust,name) )) = TYPE_ID_OF(name)

// unique type identifiers
#define type_id__type_id ((type_id)0)
extern TYPE_ID_DECL(type_id);
#define bool__type_id ((type_id)1)
extern TYPE_ID_DECL(bool);
#define byte__type_id ((type_id)2)
extern TYPE_ID_DECL(byte);
#define int8__type_id ((type_id)3)
extern TYPE_ID_DECL(int8);
#define uint8__type_id ((type_id)4)
extern TYPE_ID_DECL(uint8);
#define int16__type_id ((type_id)5)
extern TYPE_ID_DECL(int16);
#define uint16__type_id ((type_id)6)
extern TYPE_ID_DECL(uint16);
#define int32__type_id ((type_id)7)
extern TYPE_ID_DECL(int32);
#define uint32__type_id ((type_id)8)
extern TYPE_ID_DECL(uint32);
#define int64__type_id ((type_id)9)
extern TYPE_ID_DECL(int64);
#define uint64__type_id ((type_id)10)
extern TYPE_ID_DECL(uint64);
#define int128__type_id ((type_id)11)
extern TYPE_ID_DECL(int128);
#define uint128__type_id ((type_id)12)
extern TYPE_ID_DECL(uint128);
#define float32__type_id ((type_id)13)
extern TYPE_ID_DECL(float32);
#define float64__type_id ((type_id)14)
extern TYPE_ID_DECL(float64);
#define ptr__type_id ((type_id)15)
extern TYPE_ID_DECL(ptr);

// Negative values denote C types that are not Locust types.
#define size_t__type_id ((type_id)-1)
extern TYPE_ID_DECL(size_t);
#define uintptr_t__type_id ((type_id)-2)
extern TYPE_ID_DECL(uintptr_t);
#define ptrdiff_t__type_id ((type_id)-3)
extern TYPE_ID_DECL(ptrdiff_t);

// Helper macros
#define overloaded __attribute__(( overloadable ))
#define unreachable __builtin_unreachable();
#define nonnullfunc __attribute__(( nonnull ))
#define nonnullfunc_args(arg1, ...) __attribute__(( nonnull(arg1, ##__VA_ARGS__) ))
#define inlinefunc __attribute__((always_inline))
#define mallocsizefunc(arg1, ...) __attribute__(( alloc_size(arg1, ##__VA_ARGS__) ))
#define mallocfunc __attribute__(( malloc ))
#define purefunc __attribute__(( pure ))
#define purestfunc __attribute__(( const ))
#define mustuseretfunc __attribute__(( warn_unused_result ))
#define nil ((ptr)(0))

// Meta information for types
typedef struct {
    uint16 len;
    type_id args[];
} meta_args_list;

typedef struct {
    meta_args_list const * const takes;
    meta_args_list const * const returns;
    ptr name;
} meta_method_info;

typedef struct {
    type_id type;
    size_t size;
    ptr name;
} meta_field_info;

typedef struct {
    uint16 len;
    meta_method_info methods[];
} meta_methods_list;

typedef struct {
    uint16 len;
    meta_field_info fields[];
} meta_fields_list;

typedef struct {
    ptr name;
    meta_fields_list const * const fields;
    meta_methods_list const * const methods;
    const type_id type;
    size_t size;
    bool size_varies;
} meta;

#define meta__type_id ((type_id)18)
extern TYPE_ID_DECL(meta);

#define META_FIELDS_OF(type) CONCAT(type, __meta_fields)
#define META_FIELDS_DEF(type) static const meta_fields_list META_FIELDS_OF(type)
#define META_METHODS_OF(type) ·CONCAT(type, __meta_methods)
#define META_METHODS_DEF(type) static const meta_methods_list META_METHODS_OF(type)
#define META_OF(type) CONCAT(type, __meta)
#define META_DECL(typ) const meta META_OF(typ)

#define META_DEF(typ, fields_ptr, methods_ptr)                                          \
    META_DECL(typ) = {                                                                  \
        .type = TYPE_ID_OF(typ),                                                        \
        .size = sizeof(typ),                                                            \
        .size_varies = 0,                                                               \
        .name = STRINGIFY(typ),                                                         \
        .fields = fields_ptr,                                                           \
        .methods = methods_ptr                                                          \
    }

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
extern META_DECL(int128);
extern META_DECL(uint128);
extern META_DECL(float32);
extern META_DECL(float64);
extern META_DECL(ptr);

extern META_DECL(type_id);

// C (not Locust) types
extern META_DECL(size_t);
extern META_DECL(uintptr_t);
extern META_DECL(ptrdiff_t);

extern META_DECL(meta);

// Description Inquisitors
void repr(ptr item, meta const * const m);

static inline bool types_equal_size(meta const * const a, meta const * const b) {
    return (a->size == b->size);
}

static inline bool types_are_equal(meta const * const a, meta const * const b) {
    return (a->type == b->type);
}

static inline bool type_has_field(meta const * const tm, meta_field_info const * const fm) {
    if (tm->fields == 0) {
        return false;
    }
    uint16 i = 0;
    uint16 len = tm->fields->len;
    meta_field_info cmp = *fm;
    meta_field_info curr;
    ptr a, b;
    byte a_, b_;
    while (i < len) {
        curr = tm->fields->fields[i];
        if (curr.type != cmp.type) { return false; }
        if (curr.size != cmp.size) { return false; }
        a = curr.name;
        b = cmp.name;
        do {
            a_ = *(byte*)a;
            b_ = *(byte*)b;
            ++a;
            ++b;
            if (a_ == 0 && b_ == 0) { return true; }
        } while (a_ == b_);

        return false;
        ++i;
    }
    return false;
}

#define TYPED_FUNC_ARG(arg_id, type_id) __attribute__(( pointer_with_type_tag(locust,bufer_id,type_id) ))
#define TYPE_CHECK(td1, td2) assert(types_are_equal(td1, td2))


// Var
typedef struct {
    meta const * type;
    ptr data;
} var;

#define var__type_id ((type_id)16)

extern TYPE_ID_DECL(var);
extern META_DECL(var);

#define var_type_check(var, type) TYPE_CHECK(var->type, type)

void var_deinit(var *self);
void var_set(var *self, meta const * const type, ptr data);
void var_get(var *self, meta const * const type, ptr data);
#define var_init(self, type, data) var_set(self, type, data);


// Range
typedef struct {
    int64 start;
    int64 stop;
    int64 step;
} int_range;

inline void int_range_init(int_range *self, int64 start, int64 stop, int64 step);
#define int_range_deinit() ((void)0)

static inline uint64 __myabs(int64 val) {
    int64 const mask = val >> sizeof(int64) * 0x7;
    return (val + mask) ^ mask;
}

static inline bool int_range_index(int_range self, int64 i, int64 *target) {
    i = __myabs(i); // This is weird.
    i = self.start + (i*self.step);
    if ((self.step < 0 && i < self.stop) || (self.step > 0 && i > self.stop)) {
        return false;
    }
    *target = i;
    return true;
}


// Slice
// looks like a collection, often refers to a collection, but doesn't own memory.
typedef struct {
    meta const * type;
    ptr object;
    uint64 start;
    uint64 count;
    uint32 stride;
} slice;

#define slice__type_id ((type_id)99)
extern TYPE_ID_DECL(slice);
extern META_DECL(slice);

void slice_init(slice *self, meta const * const type, ptr object, uint32 start, uint32 stride, uint64 count);
void slice_deinit(slice *self);
