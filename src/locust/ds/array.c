#ifndef ARRAY_H
#include <ds/array.h>
#endif

META_FIELDS_DEF(array) = {
    2, {
        {TYPE_ID_OF(uint64), sizeof(uint64), "len"},
        {TYPE_ID_OF(byte), sizeof(ptr), "bytes"},
    }
};

META_DEF(array, & META_FIELDS_OF(array), nil);

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

array* array_create(uint64 len) {
    array *self = mem_get(& META_OF(array), 1);
    assert(self != nil);
    array_init(self, len);
    return self;
}

void array_init(array *self, uint64 len) {
    assert(self != nil);
    ptr bytes = mem_get(len);
    assert(bytes != nil);
    self->len = len;
    self->bytes = bytes;
}

void array_deinit(array *self) {
    assert(self != nil);
    mem_free(self->bytes);
}

void array_destroy(array *self) {
    assert(self != nil);
    array_deinit(self);
    mem_free(self);
}
