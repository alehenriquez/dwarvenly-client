#include <stdio.h>
#include <stdlib.h>

#include <GL3/gl3w.h>

#ifndef UTILS_H
#include "utils.h"
#endif

#define FREAD_BUF_SIZE 1024

int check_gl_errors() {
    GLenum errCode;
    int did_error = 0;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        did_error = 1;
        fprintf(stderr, "OpenGL Error: ");
        switch(errCode) {
            case GL_INVALID_ENUM:
                fprintf(stderr, "<GL_INVALID_ENUM> An invalid value was specified for an enumerated argument.\n");
            break;
            case GL_INVALID_VALUE:
                fprintf(stderr, "<GL_INVALID_VALUE> A numeric argument was out of range.\n");
            break;
            case GL_INVALID_OPERATION:
                fprintf(stderr, "<GL_INVALID_OPERATION> An operation was attempted that is not allowed in the current state.\n");
            break;
            case GL_OUT_OF_MEMORY:
                fprintf(stderr, "<GL_OUT_MEMORY> There is insufficient memory to execute the requested command.\n");
            break;
            default:
                fprintf(stderr, "<UNKNOWN_ERROR %d > This programmer doesn't know what OpenGL error was just thrown.\n", errCode);
            break;
        }
    }
    return did_error;
}

void print_matrix(float *m) {
    printf("[ %5.2f %5.2f %5.2f %5.2f\n  %5.2f %5.2f %5.2f %5.2f\n  %5.2f %5.2f %5.2f %5.2f\n  %5.2f %5.2f %5.2f %5.2f ]\n",
               m[0], m[1], m[2], m[3],    m[4], m[5], m[6], m[7],    m[8], m[9],m[10],m[11],   m[12],m[13],m[14],m[15]);
}

char *getfile(char *filename) {
    unsigned buff_len = 0;
    unsigned buff_cap = FREAD_BUF_SIZE;
    char *buff, *temp;

    buff = malloc(sizeof(char)*buff_cap);
    if (buff == NULL) {
        fprintf(stderr, "getfile(%s) error: Out of (malloc) memory.", filename);
        return NULL;
    }

    // Open file
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "getfile(%s) error: Cannot open.\n", filename);
        return NULL;
    }

    while (1) {
        buff_len += fread(&buff[buff_len], sizeof(char), FREAD_BUF_SIZE, f);

        if (feof(f)) {
            temp = realloc(buff, sizeof(char) * (buff_len+1));
            if (temp == NULL) {
                fprintf(stderr, "getfile(%s) error: Out of (final realloc) memory.", filename);
                free(buff);
                return NULL;
            }
            buff = temp;
			buff[buff_len] = 0; // null terminate the char *
			break;
        }

        if (ferror(f)) {
            fprintf(stderr, "getfile(%s) error: ferror() while reading.", filename);
            free(buff);
            return NULL;
        }

        // Ensure there is always at least FREAD_BUF_SIZE space to read into
        if ((buff_cap - buff_len) < FREAD_BUF_SIZE) {
            buff_cap = buff_len + (buff_cap - buff_len) + FREAD_BUF_SIZE;
            temp = realloc(buff, sizeof(char)*buff_cap);
            if (temp == NULL) {
				fprintf(stderr, "getfile(%s) error: Out of (realloc) memory.", filename);
				free(buff);
                return NULL;
            }
			buff = temp;
        }
    }

    // Try to close until it closes or we give up
    int close_attempts = 10;
    while (fclose(f) != 0 && close_attempts > 0) {
        --close_attempts;
    };
    if (close_attempts <= 0) {
        fprintf(stderr, "getfile(%s) error: Cannot close. Proceeding obliviously.\n", filename);
    }

    return buff;
}
