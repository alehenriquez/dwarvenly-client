#define WINDOW_H

#include <GL3/gl3w.h>
#include <GL/glfw.h>

static int w_width = 1;
static int w_height = 1;
static int w_initialized = 0;

void GLFWCALL win_on_resize(int width, int height);
void win_open(int width, int height, int r_bits, int g_bits, int b_bits, int alpha_bits, int depth_bits, int stencil_bits, int windowed);
void win_close(void);
