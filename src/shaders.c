#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SHADERS_H
#include "shaders.h"
#endif

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#define BUF_SIZE 1024

#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

shader_t file_to_shader(FILE *f) {
    assert(f != NULL);
	shader_t s = {0};
	unsigned buff_len = 0;
	GLchar *buff = (GLchar*)malloc(sizeof(GLchar)*BUF_SIZE);
	unsigned buffs_i = 0;
	GLchar **buffs = (GLchar**)malloc(sizeof(GLchar*));
	GLint *buffs_lens = (GLint*)malloc(sizeof(GLint));
	void *temp;

    while (1) {
        buff_len += fread(buff, sizeof(GLchar), BUF_SIZE-buff_len, f);

        if (feof(f)) {
			buffs[buffs_i] = buff;
			buffs_lens[buffs_i] = buff_len;
			s.buf_num = buffs_i+1;
			s.buf = buffs;
			s.buf_len = buffs_lens;
            return s;
        }

        if (ferror(f)) {
            fprintf(stderr, "Error reading shader file.");
            return s;
        }

        // Check for moving on to next buffer line
        if (buff_len >= BUF_SIZE) {
			// Persistence first
			buffs[buffs_i] = buff;
			buffs_lens[buffs_i] = buff_len;
			buffs_i += 1;

			// New line
			buff = (GLchar*)malloc(sizeof(GLchar)*BUF_SIZE);
			buff_len = 0;

            // Add another line to persisters
            temp = realloc(buffs, sizeof(GLchar*)*(buffs_i+1));
            if (temp == NULL) {
				fprintf(stderr, "Error reading shader file, out of memory.");
                return s;
            }
			buffs = (GLchar**)temp;

			// Add another line len to persisters
			temp = realloc(buffs_lens, sizeof(GLint)*(buffs_i+1));
			if (temp == NULL) {
			    fprintf(stderr, "Error reading shader file, out of memory.");
				return s;
			}
			buffs_lens = (GLint*)temp;
        }
    }
}


GLuint mk_shader(char *filename) {
    assert(filename != NULL);
    GLuint shader_id;
    unsigned int type=0;

    // Attempt to determine shader type from filename extension
    char *ext = NULL;
	ext = strrchr(filename, '.');
	if (ext == NULL) {
		fprintf(stderr, "Unable to determine shader type from filename '%s', no extension found.", filename);
		return 0;
	}

	switch(ext[1]) { // Check first char of extension.
		case 'v':
        case 'V':
            type = 1;
            break;
        case 'f':
        case 'F':
            type = 2;
            break;
        case 'g':
        case 'G':
            type = 3;
            break;
        default:
            fprintf(stderr, "Unrecognized shader extension '%s'. Either use the type keyword argument or extension must start with case insensitive 'v' 'f' or 'g' for Vertex, Fragment, and Geometry shaders respectively.", ext);
            return 0;
	}

    switch(type) {
        case 1:
            shader_id = glCreateShader(GL_VERTEX_SHADER);
            break;
        case 2:
            shader_id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case 3:
            shader_id = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        default:
            fprintf(stderr, "Unknown shader_id type. Something went catastrophically wrong with shader type detection.");
            return 0;
    }

	FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Failed to open shader file: %s", filename);
        return 0;
    }

	shader_t s = file_to_shader(f);
    fclose(f);

    if (s.buf_num < 1) {
        fprintf(stderr, "Failed to read shader source from file: %s", filename);
        return 0;
    }

    if (s.buf_num == 1 && s.buf_len[0] < 1) {
        fprintf(stderr, "Empty shader source file: %s", filename);
        return 0;
    }

    glShaderSource(shader_id, s.buf_num, (const GLchar **)s.buf, s.buf_len);
    glCompileShader(shader_id);

	GLint status = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);

	// Compile fail check
	if (status != GL_TRUE) {
		GLint log_len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
		// No log?
		if (log_len < 1) {
			fprintf(stderr, "Shader compile error: file '%s', but no information log available. Blame OpenGL.", filename);
			return 0;
		} else {
			GLchar *log = (GLchar*)malloc(log_len * sizeof(GLchar));
			// malloc failed?
			if (log == NULL) {
				fprintf(stderr, "Shader compile error: file '%s', Additionally, failed to obtain space for compile log.", filename);
				return 0;
			}

			glGetShaderInfoLog(shader_id, log_len, NULL, log);
			fprintf(stderr, "Shader compile error: file '%s', log: %s", filename, log);
			free(log);
			return 0;
		}
	}

    return shader_id;
}

void rm_shader(GLuint shader_id) {
	glDeleteShader(shader_id);
    return;
}

program_t mk_program(GLuint vertex_shader_id, GLuint fragment_shader_id, GLuint geometry_shader_id) {
	program_t p;
	p.is_compiled = 0;
	p.program_id = 0;

	// Make sure we have at least one shader
	if (vertex_shader_id == 0 && fragment_shader_id == 0 && geometry_shader_id == 0) {
		fprintf(stderr, "No valid shader ids specified for shader program creation. Need at least 1.");
		return p;
	}

	p.program_id = glCreateProgram();
	if (p.program_id == 0) {
		fprintf(stderr, "Creating the shader program failed, glCreateProgram returned 0.");
		return p;
	}

    if (vertex_shader_id != 0) {
        glAttachShader(p.program_id, vertex_shader_id);
    }
    if (fragment_shader_id != 0) {
        glAttachShader(p.program_id, fragment_shader_id);
    }
    if (geometry_shader_id != 0) {
        glAttachShader(p.program_id, geometry_shader_id);
    }

    compile_program(&p);

/*
    // \o/
    // Setup graphics integration
    DEBUG_GL
    glUseProgram(p.program_id);
    DEBUG_GL
    p.v_position = glGetAttribLocation(p.program_id, "v_position");
    DEBUG_GL
    glEnableVertexAttribArray(p.v_position);
    DEBUG_GL
    glVertexAttribPointer(p.v_position, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), BUFFER_OFFSET(0)); // DEPENDENCY: graphics.h vertex_t layout
    DEBUG_GL
    //p.v_normal = glGetAttribLocation(p.program_id, "v_normal");
    //DEBUG_GL
    //glEnableVertexAttribArray(p.v_normal);
    //DEBUG_GL
    //glVertexAttribPointer(p.v_normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), BUFFER_OFFSET(12)); // DEPENDENCY: graphics.h vertex_t layout
    //DEBUG_GL
    p.proj_matrix = glGetUniformLocation(p.program_id, "proj_matrix");
    DEBUG_GL
    //glUniformMatrix4fv(p.proj_matrix, 1, GL_TRUE, c->persp_matrix);
    DEBUG_GL
    p.view_matrix = glGetUniformLocation(p.program_id, "view_matrix");
    DEBUG_GL
    //glUniformMatrix4fv(p.view_matrix, 1, GL_TRUE, c->cam_matrix);
*/

	return p;
}

void rm_program(program_t *p) {
    assert(p != NULL);
    glDeleteProgram(p->program_id);
    return;
}

void use_program(program_t *p) {
    assert(p != NULL);
    glUseProgram(p->program_id);
    return;
}

void compile_program(program_t *p) {
    assert(p != NULL);
    p->is_compiled = 0;
    glLinkProgram(p->program_id);

    // Check for linking success
    GLint success = GL_FALSE;
    glGetProgramiv(p->program_id, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        // Get info log length
        GLint log_len = 0;
        glGetProgramiv(p->program_id, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len < 1) {
            fprintf(stderr, "Linking program %u failed. Additionally, fetching info log for the program failed.", p->program_id);
            p->program_id = 0;
            return;
        }

        // Return info log as error
        GLchar *log = (GLchar*)malloc(sizeof(GLchar)*log_len);
        if (log == NULL) {
            fprintf(stderr, "Linking program %u failed. Additionally, fetching memory for the info log also failed.", p->program_id);
            p->program_id = 0;
            return;
        }
        glGetProgramInfoLog(p->program_id, log_len, NULL, log);
        fprintf(stderr, "Linking program %u failed. Info: %s", p->program_id, log);
        free(log);
        p->program_id = 0;
        return;
    }

    // Validate program works
    success = GL_FALSE;
    glValidateProgram(p->program_id);
    glGetProgramiv(p->program_id, GL_VALIDATE_STATUS, &success);
    if (success != GL_TRUE) {
        // Get info log length
        GLint log_len = 0;
        glGetProgramiv(p->program_id, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len < 1) {
            fprintf(stderr, "Validating program %u failed. Additionally, fetching info log for the program failed.", p->program_id);
            p->program_id = 0;
            return;
        }

        // Return info log as error
        GLchar *log = (GLchar*)malloc(sizeof(GLchar)*log_len);
        if (log == NULL) {
            fprintf(stderr, "Validating program %u failed. Additionally, fetching memory for the info log also failed.", p->program_id);
            p->program_id = 0;
            return;
        }
        glGetProgramInfoLog(p->program_id, log_len, NULL, log);
        fprintf(stderr, "Validating program %u failed. Info: %s", p->program_id, log);
        free(log);
        p->program_id = 0;
        return;
    }

    p->is_compiled = 1;

	/*
	typedef struct attribs {
		GLint v_position;
		GLint v_normal;
	    GLint v_texcoords;
	} attribs_t;
	*/
    // Set up program attribs

/*    attribs_t a;
    a.v_position = glGetAttribLocation(p->program_id, "v_position");
	assert(a.v_position >= 0);
    DEBUG_GL
    a.v_normal = glGetAttribLocation(p->program_id, "v_normal");
	assert(a.v_normal >= 0);
    DEBUG_GL
    a.v_texcoords = glGetAttribLocation(p->program_id, "v_texcoords");
	assert(a.v_texcoords >= 0);
    DEBUG_GL
    p->attribs = a;
*/

	/*
	typedef struct uniforms {
	    GLint proj_matrix;
	    GLint model_matrix;
	    GLint view_matrix;
		GLint light_pos;
		GLint ambient_color;
		GLint diffuse_color;
		GLint specular_color;
		GLint colormap;
		GLint normalmap;
	} uniforms_t;
	*/
    // Set up program uniforms
/*
    uniforms_t u;
    u.proj_matrix = glGetUniformLocation(p->program_id, "proj_matrix");
	assert(u.proj_matrix >= 0);
    DEBUG_GL
    u.model_matrix = glGetUniformLocation(p->program_id, "model_matrix");
	assert(u.model_matrix >= 0);
    DEBUG_GL
    u.view_matrix = glGetUniformLocation(p->program_id, "view_matrix");
	assert(u.view_matrix >= 0);
    DEBUG_GL
	u.light_pos = glGetUniformLocation(p->program_id, "light_pos");
	assert(u.light_pos >= 0);
    DEBUG_GL
	u.ambient_color = glGetUniformLocation(p->program_id, "ambient_color");
	assert(u.ambient_color >= 0);
    DEBUG_GL
	u.diffuse_color = glGetUniformLocation(p->program_id, "diffuse_color");
	assert(u.diffuse_color >= 0);
    DEBUG_GL
	u.specular_color = glGetUniformLocation(p->program_id, "specular_color");
	assert(u.specular_color >= 0);
    DEBUG_GL
    u.colormap = glGetUniformLocation(p->program_id, "colormap");
	assert(u.colormap >= 0);
    DEBUG_GL
	u.normalmap = glGetUniformLocation(p->program_id, "normalmap");
	assert(u.normalmap >= 0);
    DEBUG_GL
    p->uniforms = u;
*/
}
