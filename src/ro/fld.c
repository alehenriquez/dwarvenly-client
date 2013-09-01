#ifndef FLD_H
#include <ro/fld.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

fld fld_create(uint16 width, uint16 height) {
	fld f;
	f.width = width;
	f.height = height;
	f.tiles = mem_get(width*height);
	error_goto_if(f.tiles == nil, "NilPtr: f.tiles; mem_get() failure");
	mem_set(f.tiles, width*height, 0);
  error:
    return f;
}