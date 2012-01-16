#include <stdio.h>

#ifndef WINDOW_H
#include "window.h"
#endif

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

void GLFWCALL win_on_resize(int width, int height) {
	if (width < 1) { width = 1; }
	if (height < 1) { height = 1; }
    w_width = width;
    w_height = height;
    glViewport(0, 0, width, height);
	return;
}

void win_open(int width, int height, int r_bits, int g_bits, int b_bits, int alpha_bits, int depth_bits, int stencil_bits, int windowed) {
    int success = 0;
    windowed = (windowed != 0) ? GLFW_WINDOW : GLFW_FULLSCREEN;

    if (w_initialized == 0) {
        success = glfwInit();
        if (success != GL_TRUE) {
            fprintf(stderr, "glfwInit() failed");
            return;
        }
        w_initialized = 1;
    }

    success = glfwOpenWindow(width, height, r_bits, g_bits, b_bits, alpha_bits, depth_bits, stencil_bits, windowed);
    if (success != GL_TRUE) {
        fprintf(stderr, "glfwOpenWindow() failed");
		glfwTerminate();
        return;
    }

    if (gl3wInit() != 0) {
        fprintf(stderr, "gl3wInit() failed");
        return;
    }

    if (!gl3wIsSupported(3, 3)) {
        fprintf(stderr, "OpenGL 3.3 not supported on this device");
        return;
    }

    glfwSetWindowSizeCallback( &win_on_resize );

    return;
}

void win_close(void) {
    glfwCloseWindow();
    return;
}
