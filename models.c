#ifndef MODELS_H
#include "models.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

#include "cJSON.h"
#include "SOIL.h"

#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))


void load_models(char *filename, settings_t *settings, program_t *program) {
    char *buff = getfile(filename);
    if (buff == NULL) {
        fprintf(stderr, "load_models(%s) failed.", filename);
        return;
    }

    cJSON *root = cJSON_Parse(buff);
    if (!root) {
        fprintf(stderr, "Error parsing %s before char: %s\n", filename, cJSON_GetErrorPtr());
        return;
    }

    int model_num = cJSON_GetArraySize(root->child);
    if (model_num < 1) {
        fprintf(stderr, "WHAT HAVE YOU DONE? There are no models in %s\n", filename);
    }

    for (int i=0; i < model_num; i++) {
        printf("loading model #%d\n", i);
        fflush(stdout);
        fflush(stderr);

        cJSON *model = cJSON_GetArrayItem(root, i);
        if (!model) {
            fprintf(stderr, "Error getting model %u in file: %s\n", i, filename);
            exit(1);
        }

        int id = cJSON_GetObjectItem(model, "id")->valueint;
        if (id == 0) {
            continue;
        }
        if (id > 255) {
            fprintf(stderr, "WARNING, model id exceeds 0->255! model number %u in file %s\n", i, filename);
            exit(1);
        }

        model_t m;

        // Setup basic info
        m.id = id;
        m.name = model->string;
        m.program = program;

        cJSON *tex_type = cJSON_GetObjectItem(model, "texture_type");
        if (!tex_type) {
            fprintf(stderr, "No texture type for model %u in file %s\n", i, filename);
            exit(1);
        }
        m.tex_type = CUBEMAP;

        cJSON *cubemap_order;
        if (m.tex_type == CUBEMAP) {
            cubemap_order = cJSON_GetObjectItem(model, "cubemap_order");
            if (!cubemap_order) {
                fprintf(stderr, "No cubemap_order for cubemap model %u in file %s\n", i, filename);
                exit(1);
            }
        }

        cJSON *texture = cJSON_GetObjectItem(model, "texture");
        if (!texture) {
            fprintf(stderr, "No texture for model %u in file %s\n", i, filename);
            exit(1);
        }
        m.texture = texture->valuestring;


        cJSON *vertices = cJSON_GetObjectItem(model, "vertices");
        int vertices_len = cJSON_GetArraySize(vertices);
        cJSON *normals = cJSON_GetObjectItem(model, "normals");
        int normals_len = cJSON_GetArraySize(normals);
        cJSON *texcoords = cJSON_GetObjectItem(model, "texcoords");
        int texcoords_len = cJSON_GetArraySize(texcoords);
        cJSON *indices = cJSON_GetObjectItem(model, "indices");
        int indices_len = cJSON_GetArraySize(indices);

        // Make sure data matches up. Assumes same number of elements per vertex all around
        if (vertices_len != normals_len || vertices_len != texcoords_len) {
            fprintf(stderr, "Model(%s) vertex data corrupted: %d vertices, %d normals, %d texcoords\n", m.name, vertices_len, normals_len, texcoords_len);
            exit(1);
        }
        m.vertices_len = vertices_len;
        m.indices_len = indices_len;

        // build up vertices
        vertex_t *verts = malloc(sizeof(vertex_t) * vertices_len);
        if (verts == NULL) {
            fprintf(stderr, "Model(%s) ran out of memory to represent %d vertex_t objects", m.name, vertices_len);
            exit(1);
        }

        for (int j=0, k=0; j < vertices_len; j += 3, ++k) {
            verts[k].x = (float)cJSON_GetArrayItem(vertices, j)->valuedouble;
            verts[k].y = (float)cJSON_GetArrayItem(vertices, j+1)->valuedouble;
            verts[k].z = (float)cJSON_GetArrayItem(vertices, j+2)->valuedouble;

            verts[k].nx = (float)cJSON_GetArrayItem(normals, j)->valuedouble;
            verts[k].ny = (float)cJSON_GetArrayItem(normals, j+1)->valuedouble;
            verts[k].nz = (float)cJSON_GetArrayItem(normals, j+2)->valuedouble;

            verts[k].u = (float)cJSON_GetArrayItem(texcoords, j)->valuedouble;
            verts[k].v = (float)cJSON_GetArrayItem(texcoords, j+1)->valuedouble;
            verts[k].w = (float)cJSON_GetArrayItem(texcoords, j+2)->valuedouble;
        }
        m.vertices = verts;

        // build up indices
        unsigned short *indies = malloc(sizeof(unsigned short) * indices_len);
        for (int l=0; l<indices_len; ++l) {
            indies[l] = (unsigned short)cJSON_GetArrayItem(indices, l)->valueint;
        }
        m.indices = indies;

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
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t)*vertices_len, m.vertices, GL_STATIC_DRAW);
        DEBUG_GL
        glVertexAttribPointer(m.program->attribs.v_position, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
        DEBUG_GL
        glEnableVertexAttribArray(m.program->attribs.v_position);
        DEBUG_GL
        glVertexAttribPointer(m.program->attribs.v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), BUFFER_OFFSET(3));
        DEBUG_GL
        glEnableVertexAttribArray(m.program->attribs.v_normal);
        DEBUG_GL
        glVertexAttribPointer(m.program->attribs.v_texcoords, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), BUFFER_OFFSET(6));
        DEBUG_GL
        glEnableVertexAttribArray(m.program->attribs.v_normal);

        // Setup IBO
        glGenBuffers(1, &m.ibo_id);
        DEBUG_GL
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo_id);
        DEBUG_GL
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices_len, m.indices, GL_STATIC_DRAW);
        DEBUG_GL

        char filename[256];
        snprintf(filename, sizeof filename, "%s/%s/%s", "data/textures", settings->texture_set, m.texture);

        if (m.tex_type == CUBEMAP) {
            printf("Loading cubemap \"%s\" with order %s",
                   filename,
                   cubemap_order->valuestring);
            m.tex_image = SOIL_load_OGL_single_cubemap(filename,
                                                       cubemap_order->valuestring,
                                                       SOIL_LOAD_AUTO,
                                                       SOIL_CREATE_NEW_ID,
                                                       SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
            if (!m.tex_image) {
                fprintf(stderr, "SOIL loading error: %s\n", SOIL_last_result());
            }
            printf(" and id %d\n", m.tex_image);
        }

        models[m.id] = m;
    }
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
