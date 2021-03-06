#define WINDOW_H

#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

extern GLFWwindow* window;

void win_on_resize(int width, int height);
void win_open(perspective *p, int width, int height, int r_bits, int g_bits, int b_bits, int alpha_bits, int depth_bits, int stencil_bits, int windowed);
void win_close(void);
