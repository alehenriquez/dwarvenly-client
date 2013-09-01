#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdio.h>
#include <stdlib.h>

#include <GL3/gl3w.h>

#ifndef UTILS_H
#include "utils.h"
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

int check_gl_errors(void) {
    GLenum errCode;
    int did_error = 0;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        did_error = 1;
        switch(errCode) {
            case GL_INVALID_ENUM:
                warn("GL_INVALID_ENUM: An invalid value was specified for an enumerated argument.\n");
            break;
            case GL_INVALID_VALUE:
                warn("GL_INVALID_VALUE: A numeric argument was out of range.\n");
            break;
            case GL_INVALID_OPERATION:
                warn("GL_INVALID_OPERATION: An operation was attempted that is not allowed in the current state.\n");
            break;
            case GL_OUT_OF_MEMORY:
                warn("GL_OUT_MEMORY: There is insufficient memory to execute the requested command.\n");
            break;
            default:
                warn("GL_UNKNOWN_ERROR: glGetError() -> %d -> This programmer doesn't know what OpenGL error was just thrown.\n", errCode);
            break;
        }
    }
    return did_error;
}

#define GETFILE_BUF_SIZE 1024

char *getfile(char *filename) {
    assert(filename != NULL);
    unsigned buff_len = 0;
    unsigned buff_cap = GETFILE_BUF_SIZE;
    char *buff = NULL;
    char *temp = NULL;
    FILE *f = NULL;

    buff = (char*)malloc(sizeof(char)*buff_cap);
    error_goto_if(buff == NULL, "getfile(%s): Out of (malloc) memory.", filename);

    // Open file
    f = fopen(filename, "r");
    error_goto_if(f == NULL, "getfile(%s): Cannot open.\n", filename);

    while (1) {
        buff_len += fread(&buff[buff_len], sizeof(char), GETFILE_BUF_SIZE, f);

        if (feof(f)) {
            // Try to do a downward realloc.
            temp = (char*)realloc(buff, sizeof(char) * (buff_len+1));
            if (temp == NULL) {
                warn("getfile(%s): downward realloc failed.", filename);
            } else {
                buff = temp;
            }
            buff[buff_len] = 0; // null terminate the char *
			break;
        }

        error_goto_if(ferror(f), "getfile(%s): ferror() while reading; errno: %s", filename, strerror(errno));

        // Ensure there is always at least FREAD_BUF_SIZE space to read into
        if ((buff_cap - buff_len) < GETFILE_BUF_SIZE) {
            buff_cap = buff_len + (buff_cap - buff_len) + GETFILE_BUF_SIZE;
            temp = (char*)realloc(buff, sizeof(char)*buff_cap);
            error_goto_if(temp == NULL, "getfile(%s): Out of (realloc) memory.", filename);
			buff = temp;
        }
    }

    warn_if(fclose(f), "getfile(%s): cannot close.\n", filename);
    return buff;

error:
    if (buff) { free(buff); }
    if (f) { warn_if(fclose(f), "getfile(%s): cannot close.\n", filename); }
    return NULL;
}
