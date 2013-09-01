#define WORLD_H

#include <stdint.h>

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef MAP_H
#include <locust/ds.map.h>
#endif

typedef struct world_tile_{
	struct world_tile_ *neighbors[8]; // 0 = N, clockwise.
	int64 x, y, z;

	unsigned char tiles[];
} world_tile;

typedef struct {
	map *tiles;
} world;

extern world the_world;
