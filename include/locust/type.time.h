#define TIME_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef ERROR_H
#include <locust/type.error.h>
#endif

typedef struct {
    int128 ns;
} time;
// approx. 5.3 sextillion (e^21) years before and after the epoch

#define time__type_id ((type_id)21)
extern TYPE_ID_DECL(time);

extern META_DECL(time);

META_GETTER_FUNC(time);

void time_init(time *self);
void time_deinit(time* self);

void time_now(time *self);

// oh so naive
static inline int128 time_to_usecs(time t) { return t.ns / 1000LL; }
static inline int128 time_to_msecs(time t) { return t.ns / 1000000LL; }
static inline int128 time_to_secs(time t) { return t.ns / 1000000000LL; }
static inline int128 time_to_mins(time t) { return t.ns / 60000000000LL; }
static inline int128 time_to_hrs(time t) { return t.ns / 3600000000000LL; }
static inline int128 time_to_days(time t) { return t.ns / 86400000000000LL; }

static inline time time_from_usecs(int128 usecs) { time ret = {.ns = usecs * 1000LL}; return ret; }
static inline time time_from_msecs(int128 msecs) { time ret = {.ns = msecs * 1000000LL}; return ret; }
static inline time time_from_secs(int128 secs) { time ret = {.ns = secs * 1000000000LL}; return ret; }
static inline time time_from_mins(int128 mins) { time ret = {.ns = mins * 60000000000LL}; return ret; }
static inline time time_from_hrs(int128 hrs) { time ret = {.ns = hrs * 3600000000000LL}; return ret; }
static inline time time_from_days(int128 days) { time ret = {.ns = days * 86400000000000LL}; return ret; }

