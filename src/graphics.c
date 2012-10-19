#include <math.h>
#include <string.h>

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

// Utilities
#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

#define PI_DIV_180 0.0174532925199



void cross(float *r, float x1, float y1, float z1, float x2, float y2, float z2) {
    r[0] = y1*z2 - y2*z1;
    r[1] = x2*z1 - x1*z2;
    r[2] = x1*y2 - x2*y1;
}


void imul_matrix(GLfloat *m1, GLfloat *m2) {
    GLfloat temp[16];
    for (int x=0; x < 4; x++) {
        for(int y=0; y < 4; y++) {
            temp[y + (x*4)] = (m1[x*4] * m2[y]) +
                              (m1[(x*4)+1] * m2[y+4]) +
                              (m1[(x*4)+2] * m2[y+8]) +
                              (m1[(x*4)+3] * m2[y+12]);
        }
    }
    memcpy(m1, temp, sizeof(GLfloat) << 4);
}

void translate(GLfloat *m, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat newm[16] = IDENTITY_MATRIX;
    newm[12] = x;
    newm[13] = y;
    newm[14] = z;

    imul_matrix(m, newm);
}

void rotate(GLfloat *m, GLfloat angle, axis ax) {
    const int cos1[3] = { 5, 0, 0 };
    const int cos2[3] = { 10, 10, 5 };
    const int sin1[3] = { 6, 2, 1 };
    const int sin2[3] = { 9, 8, 4 };
    GLfloat newm[16] = IDENTITY_MATRIX;

    newm[cos1[ax]] = (float)cos(PI_DIV_180 * angle);
    newm[sin1[ax]] = (float)-sin(PI_DIV_180 * angle);
    newm[sin2[ax]] =-newm[sin1[ax]];
    newm[cos2[ax]] = newm[cos1[ax]];

    imul_matrix(m, newm);
}

graphics_t mk_graphics(program_t *p, camera_t *c, model_t *m, float *model_mat, GLfloat *ambient_color, GLfloat *diffuse_color, GLfloat *specular_color, GLfloat *light_pos) {
	graphics_t g;

	g.model = m;
	g.model_matrix = model_mat;

    /*
    // Setup UBO
    transformation_t t;
    t.proj_matrix = c->persp_matrix;
    t.view_matrix = c->cam_matrix;
    t.model_matrix = model_mat;
    g.trans = t;

    glGenBuffers(1, &g.ubo_id);
    DEBUG_GL
    glBindBuffer(GL_UNIFORM_BUFFER, g.ubo_id);
    DEBUG_GL
    glBufferData(GL_UNIFORM_BUFFER, sizeof(transformation_t), &g.trans, GL_STATIC_DRAW);
    DEBUG_GL
    g.ubo_loc = glGetUniformBlockIndex(p->program_id, "Transformation");
    DEBUG_GL
    glUniformBlockBinding(p->program_id, g.ubo_loc, 1); // 0 here must be set by the programmer
    DEBUG_GL
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, g.ubo_id); // This is the same 0
    DEBUG_GL
    */
    g.program = p;
    g.camera = c;

	g.ambient_color[0] = ambient_color[0];
	g.ambient_color[1] = ambient_color[1];
	g.ambient_color[2] = ambient_color[2];
	g.ambient_color[3] = ambient_color[3];

	g.diffuse_color[0] = diffuse_color[0];
	g.diffuse_color[1] = diffuse_color[1];
	g.diffuse_color[2] = diffuse_color[2];
	g.diffuse_color[3] = diffuse_color[3];
	
	g.specular_color[0] = specular_color[0];
	g.specular_color[1] = specular_color[1];
	g.specular_color[2] = specular_color[2];
	g.specular_color[3] = specular_color[3];

	g.light_pos[0] = light_pos[0];
	g.light_pos[1] = light_pos[1];
	g.light_pos[2] = light_pos[2];

    return g;
}

void bind_graphics(graphics_t *g) {
    bind_model(g->model);
    glUseProgram(g->program->program_id);
    return;
}

void rm_graphics(graphics_t *g) {
    return;
}

void draw_graphics(graphics_t *g) {
	DEBUG_GL
    glUniformMatrix4fv(g->program->uniforms.model_matrix, 1, GL_FALSE, g->model_matrix);
    DEBUG_GL
    glUniformMatrix4fv(g->program->uniforms.view_matrix, 1, GL_FALSE, g->camera->cam_matrix);
    DEBUG_GL
    glUniformMatrix4fv(g->program->uniforms.proj_matrix, 1, GL_FALSE, g->camera->persp_matrix);
    DEBUG_GL
	glUniform4fv(g->program->uniforms.ambient_color, 1, g->ambient_color);
	DEBUG_GL
	glUniform4fv(g->program->uniforms.diffuse_color, 1, g->diffuse_color);
	DEBUG_GL
	glUniform4fv(g->program->uniforms.specular_color, 1, g->specular_color);
	DEBUG_GL
	glUniform1i(g->program->uniforms.colormap, 0);
	DEBUG_GL
	glUniform1i(g->program->uniforms.normalmap, 1);
	DEBUG_GL
	glUniform3fv(g->program->uniforms.light_pos, 1, g->light_pos);
	DEBUG_GL
    glDrawElements(GL_TRIANGLES, sizeof(unsigned short)*g->model->indices_len, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    DEBUG_GL
}
