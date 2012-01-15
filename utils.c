#include <stdio.h>
#include <GL3/gl3w.h>

#ifndef UTILS_H
#include "utils.h"
#endif

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
