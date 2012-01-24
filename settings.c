#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "cJSON.h"

#ifndef SETTINGS_H
#include "settings.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

settings_t load_settings(char *filename) {
    settings_t s;
    printf("Loading settings from: %s ...", filename);
    fflush(stdout);
    char *contents = getfile(filename);
    printf("file read...");
    fflush(stdout);
    if (contents == NULL) {
        fprintf(stderr, "Error loading settings for %s\n", filename);
        fflush(stderr);
        return s;
    }

    cJSON *root = cJSON_Parse(contents);
    if (!root) {
        fprintf(stderr, "Error parsing settings json before char: %s\n", cJSON_GetErrorPtr());
        return s;
    }
    s.window_width = cJSON_GetObjectItem(root, "window_width")->valueint;
    s.window_height = cJSON_GetObjectItem(root, "window_height")->valueint;

    char *tex_str = cJSON_GetObjectItem(root, "texture_set")->valuestring;
    s.texture_set = malloc(sizeof(char)*strlen(tex_str));
    if (s.texture_set == NULL) {
        fprintf(stderr, "Error loading settings for %s: Memory failure\n", filename);
        exit(1);
    }
    s.texture_set = memcpy(s.texture_set, tex_str, strlen(tex_str));
    printf("done.\n");
    fflush(stdout);

    cJSON_Delete(root);
    free(contents);
    return s;
}
