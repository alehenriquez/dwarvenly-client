#ifndef MODELS_H
#include "models.h"
#endif

model_t models[256];

#ifndef UTILS_H
#include "utils.h"
#endif

#ifndef IMAGES_H
#include "images.h"
#endif

#include <json/json.h>

#include <assert.h>
#include <string.h>


// microsoft ignored c99. Laaaaaaame.
#if _MSC_VER
#define snprintf _snprintf
#endif

static inline void json_debug(json_error_t* err) {
	assert(err != NULL);
	fprintf(stderr, "JSON error at \"%s\", line %ld col %ld\n    err: %s\n", err->source, err->line, err->column, err->text);
	fflush(stderr);
}

void load_models(char *filename, settings_t *settings, program_t *program) {
    char *buff = getfile(filename);
    if (buff == NULL) {
        fprintf(stderr, "load_models(%s) failed.", filename);
        return;
    }

	json_error_t err;
    json_t *root = json_loads(buff, 0, &err);
    if (!root) {
		json_debug(&err);
        return;
    }

    int model_num = json_object_size(root);
    if (model_num < 1) {
        fprintf(stderr, "WHAT HAVE YOU DONE? There are no models in %s\n", filename);
    }

	const char *key;
	json_t *val;
	unsigned int i = 0;
	json_t *id_;
	int id;

	json_object_foreach(root, key, val) {
		printf("loading model #%d\n", i++);
        fflush(stdout);
		
		id_ = json_object_get(val, "id");
		assert(json_is_integer(id_));
		id = (int)json_integer_value(id_);
        if (id > 255) {
            fprintf(stderr, "WARNING, model id exceeds 0->255! model number %u in file %s\n", i, filename);
            exit(1);
        }

		printf("model id #%d\n", id);
        fflush(stdout);

        model_t m;

        // Setup basic info
        m.id = id;
        m.name = key;
        m.program = program;
		m.indexed = false;
		
		json_t *tex = json_object_get(val, "texture");
		assert(json_is_string(tex));
		m.colormap_filename = json_string_value(tex);

		json_t *norm_map = json_object_get(val, "normalmap");
		assert(json_is_string(norm_map));
		m.normalmap_filename = json_string_value(norm_map);
        
		json_t *verts = json_object_get(val, "vertices");
		assert(json_is_array(verts));
		json_t *norms = json_object_get(val, "normals");
		assert(json_is_array(norms));
		json_t *tex_face_coords = json_object_get(val, "tex_face_coords");
		assert(json_is_array(tex_face_coords));
		json_t *tex_face_indices = json_object_get(val, "tex_face_indices");
		assert(json_is_array(tex_face_indices));
		
		json_t *indices = json_object_get(val, "indices");
		if (indices != NULL && json_is_array(indices)) {
			m.indexed = true;
		}

        int vertices_len = json_array_size(verts);
        int normals_len = json_array_size(norms);
		int tex_face_coords_len = json_array_size(tex_face_coords);
		int tex_face_indices_len = json_array_size(tex_face_indices);
		int indices_len;
        if (m.indexed) indices_len = json_array_size(indices);

        // Make sure data matches up. Assumes same number of elements per vertex all around
        if (vertices_len != normals_len) {
            fprintf(stderr, "Model(%s) vertex data corrupted: %d vertices, %d normals\n", m.name, vertices_len, normals_len);
            exit(1);
        }
		if (vertices_len != (tex_face_indices_len*3)) {
			fprintf(stderr, "Model(%s) vertex data corrupted: %d vertices, %d normals, %d tex_face_indices\n", m.name, vertices_len, normals_len, tex_face_indices_len);
            exit(1);
		}
        m.vertices_len = vertices_len;
        m.indices_len = indices_len;

        // build up vertices
        vertex_t *vertices = (vertex_t*)malloc(sizeof(vertex_t) * vertices_len);
        if (vertices == NULL) {
            fprintf(stderr, "Model(%s) ran out of memory to represent %d vertex objects", m.name, vertices_len);
            exit(1);
        }

		json_t *tmp;
		json_t *tmp2;
		int j, k;
		int face_index;

#define SETVAL(source, index, dest) \
	tmp = json_array_get(source, index); \
	assert(json_is_real(tmp)); \
	vertices[k].##dest = (float)json_real_value(tmp);

		for (j=0, k=0; j < vertices_len; j+=3, ++k) {
			SETVAL(verts, j, x);
			SETVAL(verts, j+1, y);
			SETVAL(verts, j+2, z);
			SETVAL(norms, j, nx);
			SETVAL(norms, j+1, ny);
			SETVAL(norms, j+2, nz);
			tmp2 = json_array_get(tex_face_indices, k);
			assert(json_is_integer(tmp2));
			face_index = (int)json_integer_value(tmp2);
			face_index *= 2; // since there are 2 texcoords for each vertex
			SETVAL(tex_face_coords, face_index, u);
			SETVAL(tex_face_coords, face_index+1, v);
		}
        m.vertices = vertices;

		unsigned short *indies;
		if (m.indexed) {
			// build up indices
			indies = (unsigned short*)malloc(sizeof(unsigned short) * indices_len);
			for (int l=0; l<indices_len; ++l) {
				tmp = json_array_get(indices, l);
				assert(json_is_integer(tmp));
				indies[l] = (unsigned short)json_integer_value(tmp);
			}
			m.indices = indies;
		}

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

#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t)*vertices_len, m.vertices, GL_STATIC_DRAW);
        DEBUG_GL
        glVertexAttribPointer(m.program->attribs.v_position, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
        DEBUG_GL
        glEnableVertexAttribArray(m.program->attribs.v_position);
        DEBUG_GL
		glVertexAttribPointer(m.program->attribs.v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)(3*sizeof(GLfloat)));
        DEBUG_GL
        glEnableVertexAttribArray(m.program->attribs.v_normal);
        DEBUG_GL
		glVertexAttribPointer(m.program->attribs.v_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)(6*sizeof(GLfloat)));
        DEBUG_GL
		glEnableVertexAttribArray(m.program->attribs.v_texcoords);

        // Setup IBO
        glGenBuffers(1, &m.ibo_id);
        DEBUG_GL
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ibo_id);
        DEBUG_GL
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices_len, m.indices, GL_STATIC_DRAW);
        DEBUG_GL

        
        char filename[4096];

		glActiveTexture(GL_TEXTURE0+0);
		DEBUG_GL
		snprintf(filename, sizeof filename, "%s/%s/%s", "data/textures", settings->texture_set, m.colormap_filename);
		m.colormap = get_image_id(filename);
		glBindTexture(GL_TEXTURE_2D, m.colormap);
		printf("Got for colormap id: %ld\n", m.colormap);
		
		glActiveTexture(GL_TEXTURE0+1);
		DEBUG_GL
		snprintf(filename, sizeof filename, "%s/%s/%s", "data/textures", settings->texture_set, m.normalmap_filename);
		m.normalmap = get_image_id(filename);
		glBindTexture(GL_TEXTURE_2D, m.normalmap);
		printf("Got for normalmap id: %ld\n", m.normalmap);
        models[m.id] = m;
    }
}

void bind_model(model_t *m) {
    glBindVertexArray(m->vao_id);
	DEBUG_GL
	glActiveTexture(GL_TEXTURE0+0);
	DEBUG_GL
	glBindTexture(GL_TEXTURE_2D, m->colormap);
	DEBUG_GL    
	glActiveTexture(GL_TEXTURE0+1);
	DEBUG_GL
	glBindTexture(GL_TEXTURE_2D, m->normalmap);
	DEBUG_GL    
}

void rm_model(model_t *m) {
    glDeleteBuffers(1, &m->v_vbo_id);
    glDeleteBuffers(1, &m->ibo_id);
}
