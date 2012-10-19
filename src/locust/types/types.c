#ifndef TYPES_H
#include <types/types.h>
#endif

META_DEF_SIMPLE(bool);
META_DEF_SIMPLE(byte);
META_DEF_SIMPLE(int8);
META_DEF_SIMPLE(uint8);
META_DEF_SIMPLE(int16);
META_DEF_SIMPLE(uint16);
META_DEF_SIMPLE(int32);
META_DEF_SIMPLE(uint32);
META_DEF_SIMPLE(int64);
META_DEF_SIMPLE(uint64);
META_DEF_SIMPLE(int128);
META_DEF_SIMPLE(uint128);
META_DEF_SIMPLE(float32);
META_DEF_SIMPLE(float64);
META_DEF_SIMPLE(ptr);

META_DEF_SIMPLE(type_id);

// C (not Locust) types
META_DEF_SIMPLE(size_t);
META_DEF_SIMPLE(uintptr_t);
META_DEF_SIMPLE(ptrdiff_t);

META_FIELDS_DEF(meta) = {
    5, {
        {TYPE_ID_OF(type_id), sizeof(type_id), "type"},
        {TYPE_ID_OF(size_t), sizeof(size_t), "size"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "name"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "fields"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "methods"},
    }
};

META_DEF(meta, & META_FIELDS_OF(meta), nil);


#include <stdio.h>

static inline void repr__basics(ptr item, const meta *m) {
	
}


void repr(ptr item, const meta *m) {
    printf("<%s:s=%zd:t=%d", m->name, m->size, m->type);


    if (m->fields != nil) {
        uint16 fields_count = m->fields->len;
        const field_info * f = &m->fields->fields[0];
		byte *p = item;
        for (uint16 i = 0; i < fields_count; ++i, ++f) {
			
            printf(" %s:s=%zd:t=%d", f->name, f->size, f->type);
			p += f->size;
        }
    }
    printf(">");
}
