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
    perspective_t p = {
        .fovy=60.0,
        .aspect=(float)1024/(float)768,
        .znear=1.0,
        .zfar=300.0
    };

    win_open(&p, 1024, 768, 8, 8, 8, 0, 24, 8, 1);
    DEBUG_GL

    cam_loc_t c_loc = {
        .x =0.0, .y =0.5, .z =-1.1,
        .fx=0.0, .fy=0.0, .fz=2.0,
        .ux=0.0, .uy=1.0, .uz=0.0
    };
    camera_t cam = mk_camera(p, c_loc);
    DEBUG_GL
    glClearColor(0.2, 0.75, 0.1, 1.0);
    DEBUG_GL
    GLuint sh1 = mk_shader("shaders/default.vert");
    DEBUG_GL
    GLuint sh2 = mk_shader("shaders/default.frag");
    DEBUG_GL
    program_t pr1 = mk_program(&cam, sh1, sh2, 0);
    DEBUG_GL
    use_program(&pr1);
    DEBUG_GL

    // Second simple object
    float vert2[24] = {
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5, -0.5,-0.5,
        0.5, -0.5,-0.5,
        0.5, 0.5,-0.5,
        -0.5, 0.5,-0.5
    };	// vertex array

    unsigned short indices2[36] = {
        0, 1, 2, // front
        0, 2, 3, //
        3, 2, 6, // top
        3, 6, 7, //
        7, 6, 5, // back
        7, 5, 4, //
        0, 4, 5, // bottom
        0, 5, 1, //
        0, 3, 7, // left
        0, 7, 4, //
        1, 5, 6, // right
        1, 6, 2 //
    };


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

    printf("v_position %i\n", pr1.attribs.v_position);

    printf("proj_matrix %i\n", pr1.uniforms.proj_matrix);
    printf("model_matrix %i\n", pr1.uniforms.model_matrix);
    printf("view_matrix %i\n", pr1.uniforms.view_matrix);

    DEBUG_GL

    // VAO setup
    model_t dirt = mk_model("data/textures/default/dirt.png", &pr1, vert2, 24, indices2, 36);
    model_t stone = mk_model("data/textures/default/stone.png", &pr1, vert2, 24, indices2, 36);
    model_t grass = mk_model("data/textures/default/grass.png", &pr1, vert2, 24, indices2, 36);
    model_t sand = mk_model("data/textures/default/sand.png", &pr1, vert2, 24, indices2, 36);
    model_t tundra = mk_model("data/textures/default/tundra.png", &pr1, vert2, 24, indices2, 36);
    model_t snow = mk_model("data/textures/default/snow.png", &pr1, vert2, 24, indices2, 36);
    model_t bog = mk_model("data/textures/default/bog.png", &pr1, vert2, 24, indices2, 36);

    printf("models:\n\tdirt %u\n\tstone %u\n\tgrass %u\n\tsand %u\n\ttundra %u\n\tsnow %u\n\tbog %u\n",
           dirt.tex_image, stone.tex_image, grass.tex_image, sand.tex_image, tundra.tex_image, snow.tex_image, bog.tex_image);


    DEBUG_GL
    graphics_t g1 = mk_graphics(&pr1, &cam, &dirt, model_mat1);
    graphics_t g2 = mk_graphics(&pr1, &cam, &stone, model_mat2);
    graphics_t g3 = mk_graphics(&pr1, &cam, &grass, model_mat3);
    graphics_t g4 = mk_graphics(&pr1, &cam, &sand, model_mat4);
    graphics_t g5 = mk_graphics(&pr1, &cam, &tundra, model_mat5);
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
