#define VOXELS_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

typedef struct {
	vec3 triangles[8];
	double vals[8];
} grid;

uint8 Polygonise(grid in, double isolevel, vec3 *out);
