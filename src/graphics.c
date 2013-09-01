#include <math.h>
#include <string.h>

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

#include <stdio.h>

const mat4 identity_matrix = IDENTITY_MATRIX;
const mat4 empty_matrix = EMPTY_MATRIX;

// Utilities
void print_matrix(mat4 m) {
    printf("[ %5.2f %5.2f %5.2f %5.2f\n  %5.2f %5.2f %5.2f %5.2f\n  %5.2f %5.2f %5.2f %5.2f\n  %5.2f %5.2f %5.2f %5.2f ]\n",
               m._[0], m._[1], m._[2], m._[3],    m._[4], m._[5], m._[6], m._[7],    m._[8], m._[9],m._[10],m._[11],   m._[12],m._[13],m._[14],m._[15]);
}



graphics_t mk_graphics(program_t *p, camera *c, model_t *m, float *model_mat, GLfloat *ambient_color, GLfloat *diffuse_color, GLfloat *specular_color, GLfloat *light_pos) {
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

//#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

void draw_graphics(graphics_t *g) {
	DEBUG_GL
    glUniformMatrix4fv(g->program->uniforms.model_matrix, 1, GL_FALSE, g->model_matrix);
    DEBUG_GL
    //glUniformMatrix4fv(g->program->uniforms.view_matrix, 1, GL_FALSE, g->camera->view_matrix._);
    DEBUG_GL
    //glUniformMatrix4fv(g->program->uniforms.proj_matrix, 1, GL_FALSE, g->camera->projection_matrix._);
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
    //glDrawElements(GL_TRIANGLES, sizeof(unsigned short)*g->model->indices_len, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    DEBUG_GL
}
