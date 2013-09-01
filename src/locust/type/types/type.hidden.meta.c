#ifndef TYPES_H
#include <locust/type.types.h>
#endif

// Negative values denote C types that are not Locust types.
TYPE_ID_DEF(size_t);
TYPE_ID_DEF(uintptr_t);
TYPE_ID_DEF(ptrdiff_t);

// C (not Locust) types
META_DEF_SIMPLE(size_t);
META_DEF_SIMPLE(uintptr_t);
META_DEF_SIMPLE(ptrdiff_t);