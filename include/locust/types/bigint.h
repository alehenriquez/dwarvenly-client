#define BIGINT_H

#ifndef TYPES_H
#include <types/types.h>
#endif

typedef struct {
    uint64 info;
    uint64 *bits;
} bigint;

TYPE_ID_DEF(bigint, 37);

extern META_DECL(bigint);

META_GETTER_FUNC(bigint);

bigint* bigint_create(uint64 len);
void bigint_init(bigint *self, uint64 len);
void bigint_deinit(bigint *self);
void bigint_destroy(bigint *self);

#define INFO_NEG_BITMASK (1UL << 63)
static inline bool bigint_is_negative(bigint *self) {
    return (self->info & INFO_NEG_BITMASK) > 0;
}

static inline uint64 bigint_bits_len(bigint *self) {
    return self->info & ~INFO_NEG_BITMASK;
}

static inline void bigint_negate(bigint *self) {
    self->info ^= INFO_NEG_BITMASK;
}

#undef INFO_NEG_BITMASK
