#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

inline void int_range_init(int_range *self, int64 start, int64 stop, int64 step) {
    error_goto_if(self == nil, "NilPtr: self");
    if ((step < 0 && stop > start) || (step > 0 && stop < start)) {
        warn("Invalid int_range: start %lld, stop %lld, step %lld", start, stop, step);
    }
    self->start = start;
    self->stop = stop;
    self->step = step;
  error:
    return;
}
