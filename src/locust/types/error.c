#ifndef ERROR_H
#include <types/error.h>
#endif

META_FIELDS_DEF(error) = {
    3, {
        {TYPE_ID_OF(uint32), sizeof(uint32), "id"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "name"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "info"},
    }
};

META_DEF(error, & META_FIELDS_OF(error), nil);

