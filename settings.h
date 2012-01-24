#define SETTINGS_H

typedef struct settings {
    int window_width;
    int window_height;
    char *texture_set;
} settings_t;

settings_t load_settings(char *filename);
