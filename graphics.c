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
#define EMPTY_MATRIX { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
#define IDENTITY_MATRIX { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
#define PI_DIV_360 0.00872664625
#define PI_DIV_180 0.0174532925199

void persp_to_matrix(float *m, float fovy, float aspect, float znear, float zfar) {
    memset(m, 0, sizeof(GLfloat) * 16);

    GLfloat range = tan(fovy * PI_DIV_360) * znear;

    m[0] = (2 * znear) / ((range * aspect) - (-range * aspect));
    m[5] = (2 * znear) / (2 * range);
    m[10] = -(zfar + znear) / (zfar - znear);
    m[11] = -1;
    m[14] = -(2 * zfar * znear) / (zfar - znear);

    return;
}

void cross(float *r, float x1, float y1, float z1, float x2, float y2, float z2) {
    r[0] = y1*z2 - y2*z1;
    r[1] = x2*z1 - x1*z2;
    r[2] = x1*y2 - x2*y1;
}

void cam_to_matrix(float *r, float x, float y, float z, float cx, float cy, float cz, float ux, float uy, float uz) {
    memset(r, 0.0, sizeof(GLfloat) * 16);

    // calculating the viewing vector
    float fwd[3] = {cx-x, cy-y, cz-z};

    float fwd_mag = sqrt(fwd[0]*fwd[0] + fwd[1]*fwd[1] + fwd[2]*fwd[2]);
    float up_mag = sqrt(ux*ux + uy*uy + uz*uz);

    // normalizing the viewing vector
    if( fwd_mag != 1) {
        fwd[0] = fwd[0]/fwd_mag;
        fwd[1] = fwd[1]/fwd_mag;
        fwd[2] = fwd[2]/fwd_mag;
    }

    // normalising the up vector.
    if( up_mag != 1 ) {
        ux = ux/up_mag;
        uy = uy/up_mag;
        uz = uz/up_mag;
    }

    float side[3], up[3];

    cross(side, fwd[0], fwd[1], fwd[2], ux, uy, uz);
    cross(up, side[0], side[1], side[2], fwd[0], fwd[1], fwd[2]);


    r[0] = side[0];
    r[4] = side[1];
    r[8] = side[2];

    r[1] = up[0];
    r[5] = up[1];
    r[9] = up[2];

    r[2] =-fwd[0];
    r[6] =-fwd[1];
    r[10]=-fwd[2];

    r[15]=1.0;

    translate(r, -x, -y, -z);
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

    newm[cos1[ax]] = cos(PI_DIV_180 * angle);
    newm[sin1[ax]] =-sin(PI_DIV_180 * angle);
    newm[sin2[ax]] =-newm[sin1[ax]];
    newm[cos2[ax]] = newm[cos1[ax]];

    imul_matrix(m, newm);
}

graphics_t mk_graphics(program_t *p, float *model_mat,
                       vertex_t *vertices, unsigned short vertices_len,
                       unsigned short *indices, unsigned int indices_len) {
	graphics_t g;

	g.vertices_len = vertices_len;
	g.indices_len = indices_len;
	g.vertices = vertices;
	g.indices = indices;
	g.model_matrix = model_mat;

	// Setup VAO
	DEBUG_GL
	glGenVertexArrays(1, &g.vao_id);
	DEBUG_GL
	glBindVertexArray(g.vao_id);
    DEBUG_GL

    // Setup VBO
	glGenBuffers(1, &g.vbo_id);
    DEBUG_GL
	glBindBuffer(GL_ARRAY_BUFFER, g.vbo_id);
	DEBUG_GL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t)*vertices_len, vertices, GL_STATIC_DRAW);
    DEBUG_GL

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
    DEBUG_GL
    glEnableVertexAttribArray(0);
    DEBUG_GL
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), BUFFER_OFFSET(12));
    DEBUG_GL
    glEnableVertexAttribArray(1);
    DEBUG_GL

    // Setup IBO
	glGenBuffers(1, &g.ibo_id);
    DEBUG_GL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.ibo_id);
    DEBUG_GL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices_len, indices, GL_STATIC_DRAW);
    DEBUG_GL

    // Setup UBO
    transformation_t t;
    t.proj_matrix = persp_matrix;
    t.view_matrix = cam_matrix;
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

    g.model_matrix_loc = glGetUniformLocation(p->program_id, "model_matrix");
    DEBUG_GL
    g.program = p;
    return g;
}

void bind_graphics(graphics_t g) {
    glUseProgram(g.program->program_id);
    glBindBuffer(GL_ARRAY_BUFFER, g.vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.ibo_id);
    glBindVertexArray(g.vao_id);
    return;
}

void rm_graphics(graphics_t g) {
    glDeleteBuffers(1, &g.vbo_id);
    glDeleteBuffers(1, &g.ibo_id);
    return;
}

void draw_graphics(program_t p, graphics_t g) {
    bind_graphics(g);
    glUniformMatrix4fv(g.model_matrix_loc, 1, GL_TRUE, g.model_matrix);
    glUniformMatrix4fv(p.view_matrix, 1, GL_FALSE, cam_matrix);
    glUniformMatrix4fv(p.proj_matrix, 1, GL_TRUE, persp_matrix);
    glDrawElements(GL_TRIANGLES, sizeof(unsigned short)*g.indices_len, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
}
