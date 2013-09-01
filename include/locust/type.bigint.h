#define BIGINT_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

typedef struct {
    uint64 info;
    uint64 *bits;
} bigint;

#define bigint__type_id ((type_id)37)
extern TYPE_ID_DECL(bigint);
extern META_DECL(bigint);

void bigint_init(bigint *self, uint64 len);
void bigint_deinit(bigint *self);

#define INFO_NEG_BITMASK (1ULL << 63)
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
