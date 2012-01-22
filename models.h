#define MODELS_H

#include <GL3/gl3w.h>
//#include <GL/glfw.h>

#ifndef SHADERS_H
#include "shaders.h"
#endif

typedef struct vertex {
    float x;
    float y;
    float z;
//    float nx;
//    float ny;
//    float nz;
//    float s0;
//    float t0;
//    float s1;
//    float t1;
//    float s2;
//    float t2;
//    float extra[4];
} vertex_t;

typedef struct model {
    // GPU side data
	GLuint v_vbo_id;
	GLuint ibo_id;
	GLuint vao_id;
	GLuint tex_image;

    // CPU side data
    unsigned short vertices_len;
    unsigned int indices_len;

    GLfloat *vertices;
	unsigned short *indices;

	program_t *program;
} model_t;

model_t mk_model(char *image_file, program_t *p,
                 GLfloat *vertices, unsigned short vertices_len,
                 unsigned short *indices, unsigned int indices_len);
void bind_model(model_t *m);
void rm_model(model_t *m);
void draw_model(model_t *m);
