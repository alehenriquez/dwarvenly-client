#define SHADERS_H

#include <stdio.h>
#include <GL3/gl3w.h>
#include <GL/glfw.h>

#ifndef CAMERA_H
#include "camera.h"
#endif

typedef struct shader {
	GLsizei buf_num;
	GLchar **buf;
	GLint *buf_len;
} shader_t;


typedef struct attribs {
    GLint v_position;
} attribs_t;

typedef struct uniforms {
    GLint proj_matrix;
    GLint model_matrix;
    GLint view_matrix;
    GLint tex_image;
} uniforms_t;


typedef struct program {
    GLuint program_id;
    int is_compiled;

    attribs_t attribs;
    uniforms_t uniforms;
} program_t;

/*
*/

shader_t file_to_shader(FILE *f);
GLuint mk_shader(char *filename);
void rm_shader(GLuint shader_id);
program_t mk_program(camera_t *c, GLuint vertex_shader_id, GLuint fragment_shader_id, GLuint geometry_shader_id);
void compile_program(program_t *p);
void rm_program(program_t *p);
void use_program(program_t *p);
