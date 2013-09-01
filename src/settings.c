#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <json/json.h>

#ifndef SETTINGS_H
#include "settings.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

settings_t load_settings(char *filename) {
    assert(filename != NULL);
	settings_t s = {0};
    info("Loading settings from: %s ...", filename);
    char *contents = getfile(filename);
    info("file read...");
    warn_goto_if(contents == NULL, "JSON error %s\n", filename);

	json_error_t err;
    json_t *root = json_loads(contents, 0, &err);
    warn_goto_if(root == NULL, "JSON error at \"%s\", line %d col %d\n    err: %s\n", err.source, err.line, err.column, err.text);
    
	json_t *width = json_object_get(root, "window_width");
    if (!json_is_integer(width)) {
        warn("JSON type error: width not integer");
        s.window_width = 1024;
    } else {
        s.window_width = (int)json_integer_value(width);
    }

    json_t *height = json_object_get(root, "window_height");
    if (!json_is_integer(height)) {
        warn("JSON type error: height not integer");
        s.window_height = 768;
    } else {
        s.window_height = (int)json_integer_value(height);    
    }
	

    json_t *texture_set = json_object_get(root, "texture_set");
    if (!json_is_string(texture_set)) {
        warn("JSON type error: texture_set not string");
        s.texture_set = "";
    } else {
        s.texture_set = json_string_value(texture_set);    
    }	
    
    debug("load_settings() done! %s\n", s.texture_set);

warn:
    if (contents) free(contents);
    return s;
}
