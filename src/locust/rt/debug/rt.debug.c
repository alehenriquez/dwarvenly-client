#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

__attribute__((__format__ (__printf__, 6, 7)))
int __log(char const * const mode, char const * const func_name, char const * const file_name, int line, int do_die, char const * const format, ...) {
	char * const extra_str = (errno == 0 ? NULL : strerror(errno));
    va_list args;
    errno = 0;

	fprintf(stderr, "[%s]: (%s:%d in %s) ", mode, file_name, line, func_name);

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	fprintf(stderr, "\n");
    fflush(stderr);

    if (do_die) {
    	abort();
    	__builtin_unreachable();
    }

    return 1;
}
