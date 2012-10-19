#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include <json/json.h>

#ifndef SETTINGS_H
#include "settings.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

static inline void json_debug(json_error_t* err) {
	assert(err != NULL);
	fprintf(stderr, "JSON error at \"%s\", line %ld col %ld\n    err: %s\n", err->source, err->line, err->column, err->text);
	fflush(stderr);
}

settings_t load_settings(char *filename) {
	settings_t s = {0};
    printf("Loading settings from: %s ...", filename);
    fflush(stdout);
    char *contents = getfile(filename);
    printf("file read...");
    fflush(stdout);
    if (contents == NULL) {
        fprintf(stderr, "JSON error %s\n", filename);
        fflush(stderr);
        return s;
    }

	json_error_t err;
    json_t *root = json_loads(contents, 0, &err);
    if (root == NULL) {
		json_debug(&err);
        return s;
    }
	json_t *width = json_object_get(root, "window_width");
	assert(json_is_integer(width));
	s.window_width = (int)json_integer_value(width);

    json_t *height = json_object_get(root, "window_height");
	assert(json_is_integer(height));
	s.window_height = (int)json_integer_value(height);

    json_t *texture_set = json_object_get(root, "texture_set");
	assert(json_is_string(texture_set));
	s.texture_set = json_string_value(texture_set);
    
    printf("done! %s\n", s.texture_set);
    fflush(stdout);

    free(contents);
    return s;
}
