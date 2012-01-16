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

    win_open(0, 0, 8, 8, 8, 0, 24, 8, 1);
    persp_to_matrix(persp_matrix, 60.0, (float)w_width / (float)w_height, 1.0, 10.0);
    cam_to_matrix(cam_matrix, 0.0,  5.0,  -5.0,
                              0.0,  0.0,   1.0,
                              0.0,  1.0,   0.0);
    glClearColor(0.9, 0.9, 0.9, 1.0);
    DEBUG_GL
    GLuint sh1 = mk_shader("shaders/default.vert");
    DEBUG_GL
    GLuint sh2 = mk_shader("shaders/default.frag");
    DEBUG_GL
    program_t pr1 = mk_program(sh1, sh2, 0);
    DEBUG_GL
    use_program(pr1);
    DEBUG_GL

    vertex_t *verts = malloc(8 * sizeof(vertex_t));
    verts[0].x = -1.0; verts[0].y = -1.0; verts[0].z = 2.0;
    verts[1].x =  1.0; verts[1].y = -1.0; verts[1].z = 2.0;
    verts[2].x = -1.0; verts[2].y =  1.0; verts[2].z = 2.0;
    verts[3].x =  1.0; verts[3].y =  1.0; verts[3].z = 2.0;
    verts[4].x = -1.0; verts[4].y = -1.0; verts[4].z = 3.0;
    verts[5].x =  1.0; verts[5].y = -1.0; verts[5].z = 3.0;
    verts[6].x = -1.0; verts[6].y =  1.0; verts[6].z = 3.0;
    verts[7].x =  1.0; verts[7].y =  1.0; verts[7].z = 3.0;

    /*for (unsigned int x = 0; x < 8; x++) {
        verts[x].x = (x & 0x1)-0.5;
        verts[x].y = (x & 0x2)-0.5;
        verts[x].z = (x & 0x4)+2.0;
//        verts[x].nx = 0.0;
//        verts[x].ny = 0.0;
//        verts[x].nz = 1.0;
    }*/

    unsigned short testi[] = {0, 1, 2,
                              1, 3, 2,
                              1, 5, 3,
                              5, 7, 3,
                              5, 4, 7,
                              4, 6, 7,
                              4, 0, 6,
                              0, 2, 6,
                              2, 3, 6,
                              3, 7, 6,
                              0, 4, 1,
                              4, 5, 1 };
    float model_mat1[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    DEBUG_GL
    graphics_t g = mk_graphics(&pr1, model_mat1, verts, 8, testi, 36);
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
    //glEnable(GL_CULL_FACE);

    printf("\nmodel matrix\n");
    print_matrix(model_mat1);
    printf("\npersp matrix\n");
    print_matrix(persp_matrix);
    printf("\ncamera matrix\n");
    print_matrix(cam_matrix);

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

        //rotate(model_mat1, 1.0, x_axis);
        //rotate(model_mat1, 0.5, y_axis);
        //rotate(model_mat1, 0.3, z_axis);
        //rotate(model_mat1, 0.3, z_axis);

        draw_graphics(pr1, g);

        glfwSwapBuffers();
        DEBUG_GL
    }

    glfwCloseWindow();
    glfwTerminate();

    printf("See you next time!\n");
    return 0;
}
