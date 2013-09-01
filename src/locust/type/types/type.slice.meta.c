#ifndef TYPES_H
#include <locust/type.types.h>
#endif


TYPE_ID_DEF(slice);

META_FIELDS_DEF(slice) = {
    4, {
    	{TYPE_ID_OF(ptr), sizeof(meta const *), "type"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "object"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "start"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "stride"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "count"},
        
    }
};

META_DEF(slice, & META_FIELDS_OF(slice), nil);