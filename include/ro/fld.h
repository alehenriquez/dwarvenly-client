#ifndef FLD_H
#define FLD_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

// FLDs are a derivative of OpenKore's condensed field information for the sake of space and speed.
// more info: http://openkore.com/index.php/Field_file_format

typedef struct {
   uint16 width;
   uint16 height;
   byte* tiles;
} fld;

/* 
a tile type is as follows in bit form: XXXX CSWP

X = unused, always 0
C = 1 if cliff else 0
S = 1 if snipable else 0
W = 1 if water else 0
P = 1 if passable else 0

Unknown file types are always: XXXX 0000
*/

enum {
	PASSABLE = 1 << 0,
	   WATER = 1 << 1,
	SNIPABLE = 1 << 2,
	   CLIFF = 1 << 3
};

static inline byte fld_get_tile(fld f, uint16 x, uint16 y) {
	return f.tiles[y * f.width + x];
}

static inline bool fld_tile_is_cliff(byte tile) {
	return tile & CLIFF;
}

static inline bool fld_tile_is_snipable(byte tile) {
	return tile & SNIPABLE;
}

static inline bool fld_tile_is_water(byte tile) {
	return tile & WATER;
}

static inline bool fld_tile_is_passable(byte tile) {
	return tile & PASSABLE;
}

fld fld_create(uint16 width, uint16 height);

#endif