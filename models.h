#define MODELS_H

#include <GL3/gl3w.h>
//#include <GL/glfw.h>

#ifndef SHADERS_H
#include "shaders.h"
#endif

#ifndef SETTINGS_H
#include "settings.h"
#endif

#ifndef SHADERS_H
#include "shaders.h"
#endif

typedef struct vertex {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    float u;
    float v;
    float w;
} vertex_t;

typedef enum tex {
    CUBEMAP
} tex_t;

typedef struct model {
    // GPU side data
	GLuint v_vbo_id;
	GLuint ibo_id;
	GLuint vao_id;
	GLuint tex_image;

    // CPU side data
    unsigned int id;
	tex_t tex_type;

    unsigned short vertices_len;
    unsigned int indices_len;

    vertex_t *vertices;
	unsigned short *indices;

	program_t *program;
	char *name;
	char *texture;
} model_t;

model_t models[256];

typedef enum model_name {
    AIR = 0,
    DIRT = 1,
    STONE = 2,
    GRASS = 128,
    SAND = 129,
    TUNDRA = 130,
    SNOW = 131,
    BOG = 132
} model_name_t;

void load_models(char *filename, settings_t *settings, program_t *program);
void bind_model(model_t *m);
void rm_model(model_t *m);
