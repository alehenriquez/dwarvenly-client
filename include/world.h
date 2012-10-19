#define WORLD_H

#include <stdint.h>

typedef struct world_tile_{
	uint64_t x;
	uint64_t y;
	unsigned char *tiles;
	struct world_tile_ *n;
	struct world_tile_ *e;
	struct world_tile_ *s;
	struct world_tile_ *w;
} world_tile;

typedef struct {
	uint64_t tiles_len;
	world_tile *tiles;
} world;

extern world the_world;
