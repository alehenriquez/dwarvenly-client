#define MODELS_H

#include <GL3/gl3w.h>
//#include <GL/glfw.h>

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef SHADERS_H
#include "shaders.h"
#endif

#ifndef SETTINGS_H
#include "settings.h"
#endif

#ifndef SHADERS_H
#include "shaders.h"
#endif

typedef struct {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
    unsigned char a, r, g, b;
} vertex_t;

typedef struct model {
    // GPU side data
	GLuint v_vbo_id;
	GLuint ibo_id;
	GLuint vao_id;
	GLint colormap;
	GLint normalmap;

    // CPU side data
    unsigned int id;

    unsigned short vertices_len;
    vertex_t *vertices;
	unsigned int indices_len;
	unsigned short *indices;

	program_t *program;
	const char *name;
	const char *colormap_filename;
	const char *normalmap_filename;

	bool indexed;
} model_t;

extern model_t models[];

typedef enum model_name {
    DIRT   = 0,
	STONE  = 1,
	SAND   = 2,
    SNOW   = 3,
	DIRT_FLOOR  = 128,
	STONE_FLOOR = 129,
	GRASS_FLOOR = 130,
	SAND_FLOOR  = 131,
	SNOW_FLOOR  = 132,
	CLAIMED_FLOOR = 253,
	CLAIMED_FLOOR_FANCY = 254,
	DEBUG_M = 255
} model_name_t;

void load_models(char *filename, settings_t *settings, program_t *program);
void bind_model(model_t *m);
void rm_model(model_t *m);
