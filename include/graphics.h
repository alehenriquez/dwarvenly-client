#define GRAPHICS_H

#include <GL3/gl3w.h>
#include <GL/glfw.h>

#ifndef SHADERS_H
#include "shaders.h"
#endif

#ifndef CAMERA_H
#include "camera.h"
#endif

#ifndef MODELS_H
#include "models.h"
#endif

#define EMPTY_MATRIX { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
#define IDENTITY_MATRIX { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

typedef struct transformation {
    float *proj_matrix;
    float *model_matrix;
    float *view_matrix;
} transformation_t;

typedef struct graphics {
    // GPU side data
    GLuint ubo_id;
    GLuint ubo_loc;

    // CPU side data
    model_t *model;
    float *model_matrix;

    transformation_t trans;
    program_t *program;
    camera_t *camera;

	GLfloat ambient_color[4];
	GLfloat diffuse_color[4];
	GLfloat specular_color[4];

	GLfloat light_pos[3];
} graphics_t;

typedef enum {
    x_axis,
    y_axis,
    z_axis
} axis;

void cross(float *r, float x1, float y1, float z1, float x2, float y2, float z2);
void imul_matrix(GLfloat *m1, GLfloat *m2);
void translate(GLfloat *m, GLfloat x, GLfloat y, GLfloat z);
void rotate(GLfloat *m, GLfloat angle, axis ax);
graphics_t mk_graphics(program_t *p, camera_t *c, model_t *m, float *model_mat, GLfloat *ambient_color, GLfloat *diffuse_color, GLfloat *specular_color, GLfloat *light_pos);
void bind_graphics(graphics_t *g);
void rm_graphics(graphics_t *g);
void draw_graphics(graphics_t *g);
