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

void cam_to_matrix(float *m, float x, float y, float z, float fx, float fy, float fz, float ux, float uy, float uz) {
    memset(m, 0, sizeof(GLfloat) * 16);

    // calculating the viewing vector
    float v[3] = {fx-x, fy-y, fz-z};

    float vmag = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    float umag = sqrt(ux*ux + uy*uy + uz*uz);

    // normalizing the viewing vector
    if( vmag != 0) {
        v[0] = v[0]/vmag;
        v[1] = v[1]/vmag;
        v[2] = v[2]/vmag;
    }

    // normalising the up vector.
    if( umag != 0 ) {
        ux = ux/umag;
        uy = uy/umag;
        uz = uz/umag;
    }

    float s[3], u[3];

    cross(s, v[0], v[1], v[2], ux, uy, uz);
    cross(u, s[0], s[1], s[2], v[0], v[1], v[2]);


    m[0] = s[0];
    m[1] = u[0];
    m[2] =-v[0];

    m[4] = s[1];
    m[5] = u[1];
    m[6] =-v[1];

    m[8] = s[2];
    m[9] = u[2];
    m[10]=-v[2];

    m[15]= 1;
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices_len, indices, GL_DYNAMIC_DRAW);
    DEBUG_GL

    g.model_matrix_loc = glGetUniformLocation(p->program_id, "model_matrix");
    DEBUG_GL
    g.program = p;
    return g;
}

void bind_graphics(graphics_t g) {
    glBindBuffer(GL_ARRAY_BUFFER, g.vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.ibo_id);
    return;
}

void rm_graphics(graphics_t g) {
    glDeleteBuffers(1, &g.vbo_id);
    glDeleteBuffers(1, &g.ibo_id);
    return;
}

void draw_graphics(graphics_t g) {
    glBindVertexArray(g.vao_id);
    glUniformMatrix4fv(g.model_matrix_loc, 1, GL_TRUE, g.model_matrix);
    glDrawElements(GL_TRIANGLES, sizeof(unsigned short)*g.indices_len, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
}
