#ifndef TYPES_H
#include <locust/type.types.h>
#endif

TYPE_ID_DEF(meta);

META_FIELDS_DEF(meta) = {
    6, {
        {TYPE_ID_OF(ptr), sizeof(ptr), "name"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "fields"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "methods"},
        {TYPE_ID_OF(type_id), sizeof(type_id), "type"},
        {TYPE_ID_OF(size_t), sizeof(size_t), "size"},
        {TYPE_ID_OF(bool), sizeof(bool), "size_varies"},
    }
};

META_DEF(meta, & META_FIELDS_OF(meta), nil);
