#ifndef IFACE_H
#include <types/iface.h>
#endif

META_FIELDS_DEF(iface) = {
    3, {
        {TYPE_ID_OF(ptr), sizeof(ptr), "data"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "type"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "iface"},
    }
};

META_DEF(iface, & META_FIELDS_OF(iface), nil);

