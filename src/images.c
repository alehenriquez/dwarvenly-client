#ifndef IMAGES_H
#include "images.h"
#endif

#ifndef MAP_H
#include <locust/ds.map.h>
#endif

#include "SOIL.h"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*typedef struct {
    ptr items;
    const meta *items_desc;
    uint32 len;
    uint32 cap;
    uint32 items_stride;
} map;


typedef struct {
    ptr items;
    const meta *items_desc;
    uint32 len;
    uint32 cap;
    uint32 items_stride;
} map;


*/


#define MAP_DEFAULT_CAP 512

map images = {.items= nil, .items_desc= & META_OF(uint32), .len=0, .cap=0, .items_stride=0};

unsigned int get_image_id(char *filename) {
	if (images.cap < 1) {
		map_init(&images, & META_OF(uint32), MAP_DEFAULT_CAP);
	}
	uint32 key = map_hash((ptr)filename, strlen(filename));
	uint32 result;
	if (map_get(&images, key, & META_OF(uint32), &result)) {
		return result;
	}

	printf("Loading image \"%s\"", filename);
	result = (uint32)SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	if (!result) {
		fprintf(stderr, "\nSOIL loading error: %s\n", SOIL_last_result());
		exit(1);
	}
	printf(" and id %d\n", result);
	map_set(&images, key, & META_OF(uint32), &result);
	return result;
}
