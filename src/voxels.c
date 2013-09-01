#ifndef VOXELS_H
#include "voxels.h"
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#include <math.h>

#include "voxels_lookup_tables.h"

// Given a grid cell and an isolevel, calculate the triangular
// facets required to represent the isosurface through the cell.
// Return the number of vertices copied, the array "triangles"
// will be loaded up with the vertices of at most 5 triangular facets,
// which is 3 vertices each, which is 3 doubles each, 45 doubles total.
// 0 will be returned if the grid cell is either totally above
// of totally below the isolevel.

uint8 Polygonise(grid in, double isolevel, vec3 *out) {
    error_goto_if(out == nil, "NilPtr: triangles");

    // Determine the index into the edge table which
    // tells us which vertices are inside of the surface
    int cubeindex = 0;
    cubeindex |= (in.vals[0] < isolevel) * 1;
    cubeindex |= (in.vals[1] < isolevel) * 2;
    cubeindex |= (in.vals[2] < isolevel) * 4;
    cubeindex |= (in.vals[3] < isolevel) * 8;
    cubeindex |= (in.vals[4] < isolevel) * 16;
    cubeindex |= (in.vals[5] < isolevel) * 32;
    cubeindex |= (in.vals[6] < isolevel) * 64;
    cubeindex |= (in.vals[7] < isolevel) * 128;
    int edge = edgeTable[cubeindex];

    // return 0 if cube is entirely in/out of the surface
    if (edge == 0) {
        return 0;
    }

    // Find the vertices where the surface intersects the cube
    vec3 triangle_cache[12];
    int i;
    vec3 p, p1, p2;
    double mu, valp1, valp2;
    static int const vi_i[12] = {1, 2, 3, 0, 5, 6, 7, 4, 4, 5, 6, 7}; // index lookup tables for VertexInterp "call" below

    for (i=0; i < 12; ++i) {
        if (edge & (1<<i)) {
            p1 = in.triangles[i & 0x8];
            p2 = in.triangles[vi_i[i]];
            valp1 = in.vals[i & 8];
            valp2 = in.vals[vi_i[i]];

            // Linearly interpolate the position where an isosurface cuts
            // an edge between two vertices, each with their own scalar value
            if (fabs(isolevel-valp1) < 0.00001) { p = p1; goto interpolated; }
            if (fabs(isolevel-valp2) < 0.00001) { p = p2; goto interpolated; }
            if (fabs(valp1-valp2) < 0.00001) { p = p1; goto interpolated; }
            mu = (isolevel - valp1) / (valp2 - valp1);
            p.x = p1.x + mu * (p2.x - p1.x);
            p.y = p1.y + mu * (p2.y - p1.y);
            p.z = p1.z + mu * (p2.z - p1.z);

          interpolated:
            triangle_cache[i].x = p.x;
            triangle_cache[i].y = p.y;
            triangle_cache[i].z = p.z;
        }
    }

    // Create the triangles
    int8 const * tri = &triTable[cubeindex][0];
    int8 tri_v;
    i=0;
    while ((tri_v = *tri++) != -1) {
        out[i++] = triangle_cache[tri_v];
    }

    return i;

  error:
    return 0;
}
