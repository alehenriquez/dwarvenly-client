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

    perspective_t p = {
        .fovy=60.0,
        .aspect=(float)s.window_width/(float)s.window_height,
        .znear=1.0,
        .zfar=300.0
    };

    win_open(&p, s.window_width, s.window_height, 8, 8, 8, 0, 24, 8, 1);
    DEBUG_GL

    cam_loc_t c_loc = {
        .x =0.0, .y =1.5, .z =-0.1,
        .fx=0.0, .fy=0.0, .fz=2.0,
        .ux=0.0, .uy=1.0, .uz=0.0
    };
    camera_t cam = mk_camera(p, c_loc);
    DEBUG_GL
    glClearColor(0.8, 0.8, 0.8, 1.0);
    DEBUG_GL
    GLuint sh1 = mk_shader("shaders/default.vert");
    DEBUG_GL
    GLuint sh2 = mk_shader("shaders/default.frag");
    DEBUG_GL
    program_t pr1 = mk_program(&cam, sh1, sh2, 0);
    DEBUG_GL
    use_program(&pr1);
    DEBUG_GL

    printf("v_position %i\n", pr1.attribs.v_position);
    printf("v_normal %i\n", pr1.attribs.v_normal);
    printf("v_texcoords %i\n", pr1.attribs.v_texcoords);
    printf("proj_matrix %i\n", pr1.uniforms.proj_matrix);
    printf("model_matrix %i\n", pr1.uniforms.model_matrix);
    printf("view_matrix %i\n", pr1.uniforms.view_matrix);

    load_models("data/models.json", &s, &pr1);
    DEBUG_GL

    // VAO setup
    printf("models:\n\tdirt %u\n\tstone %u\n\tgrass %u\n\tsand %u\n\ttundra %u\n\tsnow %u\n\tbog %u\n",
           models[DIRT].tex_image,
           models[STONE].tex_image,
           models[GRASS].tex_image,
           models[SAND].tex_image,
           models[TUNDRA].tex_image,
           models[SNOW].tex_image,
           models[BOG].tex_image);

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
    graphics_t g1 = mk_graphics(&pr1, &cam, &models[DIRT], model_mat1);
    graphics_t g2 = mk_graphics(&pr1, &cam, &models[STONE], model_mat2);
    graphics_t g3 = mk_graphics(&pr1, &cam, &models[GRASS], model_mat3);
    graphics_t g4 = mk_graphics(&pr1, &cam, &models[SAND], model_mat4);
    graphics_t g5 = mk_graphics(&pr1, &cam, &models[TUNDRA], model_mat5);
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
    glEnable(GL_CULL_FACE);


    glEnable(GL_TEXTURE_CUBE_MAP);
    //glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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
        //bind_graphics(&g3);
        DEBUG_GL
        //draw_graphics(&g3);
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
