#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdio.h>

#ifndef WINDOW_H
#include "window.h"
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

unsigned int w_width;
unsigned int w_height;
int w_initialized;
perspective *persp;
GLFWwindow* window;

void win_on_resize(int width, int height) {
	if (width < 1) { width = 1; }
	if (height < 1) { height = 1; }
    w_width = width;
    w_height = height;
    warn_if(persp == NULL, "Perspective poionter lost...");
    persp->aspect = (float)w_width / (float)w_height;
    glViewport(0, 0, width, height);
	return;
}

void win_open(perspective *p, int width, int height, int r_bits, int g_bits, int b_bits, int alpha_bits, int depth_bits, int stencil_bits, int windowed) {
    assert(p != NULL);
    int success = 0;
    persp = p;
    width = (width > 800) ? width : 800;
    height = (height > 600) ? height: 600;

    if (w_initialized == 0) {
        success = glfwInit();
        error_goto_if(success != GL_TRUE, "glfwInit() failed");
        w_initialized = 1;
    }

	glfwWindowHint(GLFW_SAMPLES, 16);
    window = glfwCreateWindow(width, height, "Dwarvenly", NULL, NULL);
    if (window == NULL) {
        error("glfwCreateWindow() failed");
		glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    error_goto_if(gl3wInit() != 0, "gl3wInit() failed");
    error_goto_if(!gl3wIsSupported(3, 3), "OpenGL 3.3 not supported on this device");
    glfwSetWindowSizeCallback(window, &win_on_resize );
    debug("w_width %5.3f w_height %5.3f\n", (float)w_width, (float)w_height);

error:
    return;
}

void win_close(void) {
    glfwDestroyWindow(window);
    return;
}
