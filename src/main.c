#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GL3/gl3w.h>
#include <GL/glfw.h>

#ifndef WINDOW_H
#include "window.h"
#endif

#ifndef CAMERA_H
#include "camera.h"
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

#ifndef SETTINGS_H
#include "settings.h"
#endif


static int running = 1;

int main() {
    settings_t s = load_settings("data/settings.json");
	printf("DERP %s\n", s.texture_set);
    perspective_t p = {
        60.0, //fovy
        (float)s.window_width/(float)s.window_height, //aspect
        1.0, //znear
        300.0 //zfar
    };

    win_open(&p, s.window_width, s.window_height, 8, 8, 8, 0, 24, 8, 1);
    DEBUG_GL

    cam_loc_t c_loc = {
        0.0f, 1.5f, -0.1f, // x y z
        0.0f, 0.0f,  2.0f, // fx fy fz
        0.0f, 1.0f,  0.0f  // ux uy uz
    };

	GLfloat light_pos[] = {-20.0, -5.0, 80.0};
	GLfloat ambient_color[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat diffuse_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat specular_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    camera_t cam = mk_camera(p, c_loc);
    DEBUG_GL
    glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
    DEBUG_GL
	GLuint sh1 = mk_shader("data/shaders/floor.vert");
    DEBUG_GL
    GLuint sh2 = mk_shader("data/shaders/floor.frag");
    DEBUG_GL
    program_t pr1 = mk_program(&cam, sh1, sh2, 0);
    DEBUG_GL
    use_program(&pr1);
    DEBUG_GL

    load_models("data/models.json", &s, &pr1);
    DEBUG_GL

    // VAO setup
    printf("models:\n\tdebug %u\n\tdirt %u\n\tstone %u\n\tsand %u\n\tsnow %u\n",
			models[DEBUG_M].colormap,
           models[DIRT].colormap,
           models[STONE].colormap,
           models[SAND].colormap,
           models[SNOW].colormap);

    float model_mat1[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -2.0, 0.0, 4.0, 1.0
    };

    float model_mat2[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -1.0, 0.0, 4.0, 1.0
    };

    float model_mat3[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 4.0, 1.0
    };

    float model_mat4[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 4.0, 1.0
    };

    float model_mat5[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        2.0, 0.0, 4.0, 1.0
    };

    DEBUG_GL
    graphics_t g1 = mk_graphics(&pr1, &cam, &models[STONE], model_mat1, ambient_color, diffuse_color, specular_color, light_pos);
    graphics_t g2 = mk_graphics(&pr1, &cam, &models[DIRT], model_mat2, ambient_color, diffuse_color, specular_color, light_pos);
    graphics_t g3 = mk_graphics(&pr1, &cam, &models[SNOW], model_mat3, ambient_color, diffuse_color, specular_color, light_pos);
    graphics_t g4 = mk_graphics(&pr1, &cam, &models[CLAIMED_FLOOR_FANCY], model_mat4, ambient_color, diffuse_color, specular_color, light_pos);
    graphics_t g5 = mk_graphics(&pr1, &cam, &models[CLAIMED_FLOOR], model_mat5, ambient_color, diffuse_color, specular_color, light_pos);
    DEBUG_GL
    glBindVertexArray(0);
    DEBUG_GL


    printf("\nmodel matrix\n");
    print_matrix(model_mat2);
    printf("\npersp matrix\n");
    print_matrix(cam.persp_matrix);
    printf("\ncamera matrix\n");
    print_matrix(cam.cam_matrix);


    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe debug
    //glEnable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);

    double curr = glfwGetTime();
    double prev = curr;
    double dt = 0.0;
    double phys_accum = 0.0;
    double phys_step = 0.01;
    double nextpolation = 0.0;
    double prevpolation = 0.0;

    double debug_accum = 0.0;
    double debug_step = 10.0;

    DEBUG_GL

    while (running) {
        DEBUG_GL
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prev = curr;
        curr = glfwGetTime();
        dt = curr - prev;

        phys_accum += dt;
        while(phys_accum >= phys_step) {
            // PHYSICS
            phys_accum -= phys_step;
        }

        debug_accum += dt;
        if (debug_accum >= debug_step) {
            debug_accum = 0.0;
            printf("\n-----------------------------------\n");
            printf("model matrix\n");
            print_matrix(model_mat2);
            printf("\npersp matrix\n");
            print_matrix(cam.persp_matrix);
            printf("\ncamera matrix\n");
            print_matrix(cam.cam_matrix);

        }

        nextpolation = (phys_accum / phys_step);
        prevpolation = 1 - nextpolation;

        DEBUG_GL
        bind_graphics(&g1);
        DEBUG_GL
        draw_graphics(&g1);
        DEBUG_GL
        bind_graphics(&g2);
        DEBUG_GL
        draw_graphics(&g2);
        DEBUG_GL
        bind_graphics(&g3);
        DEBUG_GL
        draw_graphics(&g3);
        DEBUG_GL
        bind_graphics(&g4);
        DEBUG_GL
        draw_graphics(&g4);
        DEBUG_GL
        bind_graphics(&g5);
        DEBUG_GL
        draw_graphics(&g5);
        DEBUG_GL
        glBindVertexArray(0);
        DEBUG_GL
        glfwSwapBuffers();
    }

    glfwCloseWindow();
    glfwTerminate();

    printf("See you next time!\n");
    return 0;
}
