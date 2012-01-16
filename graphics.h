#define GRAPHICS_H

#include <GL3/gl3w.h>
#include <GL/glfw.h>

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

typedef struct transformation {
    float *proj_matrix;
    float *model_matrix;
    float *view_matrix;
} transformation_t;

typedef struct graphics {
    // GPU side data
	GLuint vbo_id;
	GLuint ibo_id;
	GLuint vao_id;
    GLuint model_matrix_loc;
    GLuint ubo_id;
    GLuint ubo_loc;

    // CPU side data
    unsigned short vertices_len;
    unsigned int indices_len;

    vertex_t *vertices;
	unsigned short *indices;

    float *model_matrix;
    transformation_t trans;
    program_t *program;
} graphics_t;

typedef struct camera {
    double x;
    double y;
    double z;
    double fx;
    double fy;
    double fz;
    double ux;
    double uy;
    double uz;

    double x_prev;
    double y_prev;
    double z_prev;

    double fx_prev;
    double fy_prev;
    double fz_prev;
} camera_t;

typedef enum {
    x_axis,
    y_axis,
    z_axis
} axis;

static float persp_matrix[16] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
static float cam_matrix[16] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

void persp_to_matrix(float *m, float fovy, float aspect, float znear, float zfar);
void cam_to_matrix(float *m, float x, float y, float z, float fx, float fy, float fz, float ux, float uy, float uz);
void imul_matrix(GLfloat *m1, GLfloat *m2);
void translate(GLfloat *m, GLfloat x, GLfloat y, GLfloat z);
void rotate(GLfloat *m, GLfloat angle, axis ax);
graphics_t mk_graphics(program_t *p, float *model_mat, vertex_t *vertices, unsigned short vertices_len, unsigned short *indices, unsigned int indices_len);
void bind_graphics(graphics_t g);
void rm_graphics(graphics_t g);
void draw_graphics(graphics_t g);
