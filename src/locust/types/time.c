
#ifndef _WIN32
#define time __get_time
// The above is a dirty hack due to stupid people in stupid standards
// naming functions with obvious type names. Pff.
#include <sys/time.h>
#undef time
#endif

#ifndef TIME_H
#include <types/time.h>
#endif

META_FIELDS_DEF(time) = {
    1, {
        {TYPE_ID_OF(int128), sizeof(int128), "ns"},
    }
};

META_DEF_SIMPLE(time);


#ifndef SYS_H
#include <rt/sys.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#define sec_to_ns(sec) (sec*1000000000)
#define usec_to_ns(usec) (usec*1000)

error e_bad_time = { .id = 300, .name = "bad_time", .info = "A bad time value was returned from system clock" };

time* time_create(void) {
    time *self = mem_get(& META_OF(time), 1);
    assert(self != nil);
    time_init(self);
    return self;
}

void time_init(time *self) {
    assert(self != nil);
    self->ns = 0;
}

void time_deinit(time* self) {
    assert(self != nil);
    self->ns = 0;
}

void time_destroy(time* self) {
    assert(self != nil);
    time_deinit(self);
    mem_free(self);
}

// TODO: Use monotonic timers if possible. Maybe separate to time and timer impl?
void time_now(time *self) {
    assert(self != nil);
    // Platform specific time fetching.
#ifdef _WIN32
	FILETIME ft;
	uint64 tmp_t;
	GetSystemTimeAsFileTime(&ft);
	tmp_t = (uint64)ft.dwHighDateTime;
	tmp_t <<= 32;
	tmp_t |= (uint64)ft.dwLowDateTime;
	self->ns = tmp_t-116444736000000000ULL;
#else
    struct timeval tv;
    assert(gettimeofday(&tv, nil) == 0);
    if (tv.tv_sec < 0) {
        tv.tv_sec = 0;
    }
    if (tv.tv_usec < 0) {
        tv.tv_usec = 0;
    }
    self->ns = sec_to_ns((uint64)tv.tv_sec) + usec_to_ns(tv.tv_usec);
#endif
}
