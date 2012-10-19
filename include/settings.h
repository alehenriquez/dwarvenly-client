#define SETTINGS_H

typedef struct settings {
    int window_width;
    int window_height;
    const char *texture_set;
} settings_t;

settings_t load_settings(char *filename);
