#ifndef ARRAY_H
#include <locust/ds.array.h>
#endif

TYPE_ID_DEF(array);

META_FIELDS_DEF(array) = {
    3, {
        {TYPE_ID_OF(meta), sizeof(meta*), "type"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "len"},
        {TYPE_ID_OF(byte), sizeof(ptr), "bytes"},
    }
};

META_DECL(array) = { .type = TYPE_ID_OF(array), .size = 00000, .size_varies = true, .name = STRINGIFY(array), .fields = & META_FIELDS_OF(array), .methods = nil };
