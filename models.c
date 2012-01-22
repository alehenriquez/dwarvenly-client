#ifndef MODELS_H
#include "models.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif


#include "SOIL.h"

#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

model_t mk_model(char *image_file, program_t *p,
                 GLfloat *vertices, unsigned short vertices_len,
                 unsigned short *indices, unsigned int indices_len) {
	model_t m;

	m.vertices_len = vertices_len;
	m.indices_len = indices_len;

	m.vertices = vertices;
	m.indices = indices;

    m.program = p;

	// Setup VAO
	DEBUG_GL
	glGenVertexArrays(1, &m.vao_id);
	DEBUG_GL
	glBindVertexArray(m.vao_id);
    DEBUG_GL

    // Setup vertices vbo
	glGenBuffers(1, &m.v_vbo_id);
    DEBUG_GL
    glBindBuffer(GL_ARRAY_BUFFER, m.v_vbo_id);
	DEBUG_GL
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_len, m.vertices, GL_STATIC_DRAW);
    DEBUG_GL
    glVertexAttribPointer(m.program->attribs.v_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    DEBUG_GL
	glEnableVertexAttribArray(m.program->attribs.v_position);
    DEBUG_GL

    // Setup IBO
	glGenBuffers(1, &m.ibo_id);
    DEBUG_GL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo_id);
    DEBUG_GL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices_len, m.indices, GL_STATIC_DRAW);
    DEBUG_GL

    m.tex_image = SOIL_load_OGL_single_cubemap(image_file, "EWUDNS", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    return m;
}

void bind_model(model_t *m) {
    glBindVertexArray(m->vao_id);
    DEBUG_GL
    glActiveTexture(GL_TEXTURE0 + 0);
    DEBUG_GL
    glBindTexture(GL_TEXTURE_CUBE_MAP, m->tex_image);
    DEBUG_GL
}

void rm_model(model_t *m) {
    glDeleteBuffers(1, &m->v_vbo_id);
    glDeleteBuffers(1, &m->ibo_id);
}

void draw_model(model_t *m) {
    glDrawElements(GL_TRIANGLES, sizeof(unsigned short)*m->indices_len, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
}
