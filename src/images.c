#ifndef IMAGES_H
#include "images.h"
#endif

#include "SOIL.h"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef TYPES_H
#include "types/types.h"
#endif

#ifndef MAP_H
#include "ds/map.h"
#endif

map images = {0};



unsigned int get_image_id(char *filename) {
	if (images.cap < 1) {
		map_init(&images, MAP_DEFAULT_CAP);
	}
	uint32 key = map_hash((ptr)filename, strlen(filename));
	unsigned int result;
	if (map_get(&images, key, &result)) {
		return result;
	}

	printf("Loading image \"%s\"", filename);
	result = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_POWER_OF_TWO);
	if (!result) {
		fprintf(stderr, "\nSOIL loading error: %s\n", SOIL_last_result());
		exit(1);
	}
	printf(" and id %d\n", result);
	map_set(&images, key, &result);
	return result;
}
