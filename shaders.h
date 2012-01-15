#define SHADERS_H

#include <stdio.h>
#include <GL3/gl3w.h>
#include <GL/glfw.h>

typedef struct shader {
	GLsizei buf_num;
	GLchar **buf;
	GLint *buf_len;
} shader_t;

typedef struct program {
    GLuint program_id;
    GLuint v_position;
    GLuint v_normal;
    GLuint proj_matrix;
    GLuint view_matrix;
} program_t;

shader_t file_to_shader(FILE *f);
GLuint mk_shader(char *filename);
void rm_shader(GLuint shader_id);
program_t mk_program(GLuint vertex_shader_id, GLuint fragment_shader_id, GLuint geometry_shader_id);
void rm_program(program_t p);
void use_program(program_t p);
