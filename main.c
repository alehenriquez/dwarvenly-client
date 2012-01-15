#include <stdio.h>
#include <stdlib.h>
#include <GL3/gl3w.h>
#include <GL/glfw.h>

#ifndef WINDOW_H
#include "window.h"
#endif

#ifndef SHADERS_H
#include "shaders.h"
#endif

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

#ifndef UTILS_H
#include "utils.h"
#endif


static int running = 1;



int main() {
    persp_to_matrix(persp_matrix, 60.0, 1.0, 0.1, 100.0);
    cam_to_matrix(cam_matrix, 0.0,  5.0,  -5.0,
                              0.0,  0.0,   0.0,
                              0.0,  1.0,   0.0);
    win_open(0, 0, 8, 8, 8, 0, 24, 8, 1);
    DEBUG_GL
    GLuint sh1 = mk_shader("shaders/default.vert");
    DEBUG_GL
    GLuint sh2 = mk_shader("shaders/default.frag");
    DEBUG_GL
    program_t pr1 = mk_program(sh1, sh2, 0);
    DEBUG_GL
    use_program(pr1);

    float test[] = {0.0, 0.0, 0.0,
                    0.0, 0.0, 1.0,
                    0.0, 1.0, 0.0,
                    0.0, 1.0, 1.0,
                    1.0, 0.0, 0.0,
                    1.0, 0.0, 1.0,
                    1.0, 1.0, 0.0,
                    1.0, 1.0, 1.0 };

    vertex_t *verts = malloc(8 * sizeof(vertex_t));
    for (unsigned int x = 0; x < 8; x++) {
        verts[x].x = (x & 1)+0.0;
        verts[x].y = (x & 2)+0.0;
        verts[x].z = (x & 4)+0.0;
        verts[x].nx = 0.0;
        verts[x].ny = 0.0;
        verts[x].nz = -1.0;
    }

    unsigned short testi[] = {0, 1, 2,
                              0, 2, 3,
                              0, 3, 4 };
    float model_mat1[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    DEBUG_GL
    graphics_t g = mk_graphics(&pr1, model_mat1, verts, 8, testi, 9);
    DEBUG_GL

    double curr = glfwGetTime();
    double prev = curr;
    double dt = 0.0;
    double phys_accum = 0.0;
    double phys_step = 0.01;
    double nextpolation = 0.0;
    double prevpolation = 0.0;

    DEBUG_GL

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe debug
    glEnable(GL_CULL_FACE);
    glClearColor(0.8, 0.8, 0.8, 1.0);

    while(running) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prev = curr;
        curr = glfwGetTime();
        dt = curr - prev;

        phys_accum += dt;
        while(phys_accum >= phys_step) {
            // PHYSICS
            phys_accum -= phys_step;
        }

        nextpolation = (phys_accum / phys_step);
        prevpolation = 1 - nextpolation;

        glUniformMatrix4fv(pr1.view_matrix, 1, GL_TRUE, cam_matrix);
        draw_graphics(g);
        /* glLoadIdentity(); */
        /*
		gluLookAt(cam.x*nextpolation + cam.x_prev*prevpolation,
                  cam.y*nextpolation + cam.y_prev*prevpolation,
                  cam.x*nextpolation + cam.z_prev*prevpolation,
                  cam.fx*nextpolation + cam.fx_prev*prevpolation,
                  cam.fy*nextpolation + cam.fy_prev*prevpolation,
                  cam.fz*nextpolation + cam.fz_prev*prevpolation,
                  cam.ux,
                  cam.uy,
                  cam.uz);
		*/

        /* NORMAL shit with a PERSPECTIVE projection */

        /* UI shit with an ORTHO projection */
        /*
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w_width, 0, w_height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        */

        glfwSwapBuffers();
        DEBUG_GL
    }

    glfwCloseWindow();
    glfwTerminate();

    printf("See you next time!\n");
    return 0;
}
