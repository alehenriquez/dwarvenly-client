#ifndef VAR_H
#include <types/var.h>
#endif

META_FIELDS_DEF(var) = {
    2, {
        {TYPE_ID_OF(ptr), sizeof(const meta*), "type"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "data"},
    }
};

META_DEF(var, & META_FIELDS_OF(var), nil);

