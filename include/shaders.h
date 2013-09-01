#define SHADERS_H

#include <stdio.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

/*
#ifndef GRAPHICS_H
#include "graphics.h"
#endif
*/

typedef struct shader {
	GLsizei buf_num;
	GLchar **buf;
	GLint *buf_len;
} shader_t;

typedef struct attribs {
    GLint v_position;
    GLint v_normal;
    GLint v_texcoords;
} attribs_t;

typedef struct uniforms {
    GLint proj_matrix;
    GLint model_matrix;
    GLint view_matrix;
	GLint light_pos;
	GLint ambient_color;
	GLint diffuse_color;
	GLint specular_color;
	GLint colormap;
	GLint normalmap;
} uniforms_t;

typedef struct program {
    GLuint program_id;
    int is_compiled;

    attribs_t attribs;
    uniforms_t uniforms;
} program_t;


shader_t file_to_shader(FILE *f);
GLuint mk_shader(char *filename);
void rm_shader(GLuint shader_id);
program_t mk_program(GLuint vertex_shader_id, GLuint fragment_shader_id, GLuint geometry_shader_id);
void compile_program(program_t *p);
void rm_program(program_t *p);
void use_program(program_t *p);
