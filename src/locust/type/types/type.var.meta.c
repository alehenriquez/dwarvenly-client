#ifndef TYPES_H
#include <locust/type.types.h>
#endif

TYPE_ID_DEF(var);

META_FIELDS_DEF(var) = {
    2, {
        {TYPE_ID_OF(ptr), sizeof(meta const *), "type"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "data"},
    }
};

META_DEF(var, & META_FIELDS_OF(var), nil);
