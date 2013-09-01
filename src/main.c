#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

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

#ifndef SETTINGS_H
#include "settings.h"
#endif

#ifndef GND_H
#include <ro/gnd.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

static int running = 1;

int main(int c, char**v) {
    settings_t s = load_settings("data/settings.json");
    printf("DERP %s w %d h %d\n", s.texture_set, s.window_width, s.window_height);

    //gnd g = gnd_load("data/rodata/alberta.gnd");
    //gnd g = gnd_load("data/rodata/prontera.gnd");

	GLfloat ambient_color[] = {0.1f, 0.1f, 0.1f, 1.0f};
    camera cam;
    cam.p.fovy = 60.0f;
    cam.p.aspect = s.window_width / s.window_height;
    cam.p.znear = 1.0f;
    cam.p.zfar = 1000.0f;
    cam.pos.x = 0.0f;
    cam.pos.y = 0.0f;
    cam.pos.z = 1.5f;
    cam.fwd.x = 0.0f;
    cam.fwd.y = 0.0f;
    cam.fwd.z = 2.0f;
    cam.up.x = 0.0f;
    cam.up.y = 1.0f;
    cam.up.z = 0.0f;
    cam.projection_matrix = mk_projection_matrix(cam.p);
    cam.view_matrix = mk_view_matrix(cam.pos, cam.fwd, cam.up);

    win_open(&cam.p, s.window_width, s.window_height, 8, 8, 8, 0, 24, 8, 1);
    DEBUG_GL

    DEBUG_GL
    glClearColor(0.40f, 0.22f, 0.45f, 1.0f);
    DEBUG_GL
	GLuint sh1 = mk_shader("data/shaders/floor2.vert");
    DEBUG_GL
    GLuint sh2 = mk_shader("data/shaders/floor2.frag");
    DEBUG_GL
    program_t pr1 = mk_program(sh1, sh2, 0);
    DEBUG_GL
    use_program(&pr1);
    DEBUG_GL

    /*
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
        -2.0, 0.0,-2.0, 1.0
    };

    float model_mat2[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -1.0, 0.0, 22.0, 1.0
    };

    float model_mat3[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 2.0, 1.0
    };

    float model_mat4[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 22.0, 1.0
    };

    float model_mat5[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        2.0, 0.0, -22.0, 1.0
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
    */

    printf("\ncamera proje matrix\n");
    print_matrix(cam.projection_matrix);
    printf("\ncamera view matrix\n");
    print_matrix(cam.view_matrix);


    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe debug
    //glEnable(GL_CULL_FACE);

    //glEnable(GL_TEXTURE_2D);

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
    //gnd_load_gl(&g, &pr1, &cam, ambient_color);
    DEBUG_GL

    vertex tri_v[3];
    tri_v[0].diffuse.a = 1.0f;
    tri_v[0].diffuse.r = 1.0f;
    tri_v[0].diffuse.g = 1.0f;
    tri_v[0].diffuse.b = 1.0f;

    tri_v[0].x = -1.0;
    tri_v[0].y = -1.0;
    tri_v[0].z = 2.0;

    tri_v[1].diffuse.a = 1.0f;
    tri_v[1].diffuse.r = 1.0f;
    tri_v[1].diffuse.g = 0.0f;
    tri_v[1].diffuse.b = 0.0f;

    tri_v[1].x = 1.0;
    tri_v[1].y = -1.0;
    tri_v[1].z = 2.0;

    tri_v[2].diffuse.a = 1.0f;
    tri_v[2].diffuse.r = 0.0f;
    tri_v[2].diffuse.g = 1.0f;
    tri_v[2].diffuse.b = 0.0f;

    tri_v[2].x = 1.0;
    tri_v[2].y = 1.0;
    tri_v[2].z = 2.0;

    uint32 v_pos = glGetAttribLocation(pr1.program_id, "v_position");
    assert(v_pos >= 0);
    DEBUG_GL
    uint32 v_diffuse = glGetAttribLocation(pr1.program_id, "v_diffuse");
    assert(v_diffuse >= 0);
    DEBUG_GL

    uint32 tri_vbo_id, tri_vao_id;
    glGenBuffers(1, &tri_vbo_id);
    DEBUG_GL
    glBindBuffer(GL_ARRAY_BUFFER, tri_vbo_id);
    DEBUG_GL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*3, &tri_v, GL_STATIC_DRAW);
    DEBUG_GL
    
    glGenVertexArrays(1, &tri_vao_id);
    DEBUG_GL
    glBindVertexArray(tri_vao_id);
    DEBUG_GL

    glEnableVertexAttribArray(v_pos);
    DEBUG_GL
    glVertexAttribPointer(v_pos, 3, GL_FLOAT, false, sizeof(vertex), 0);
    DEBUG_GL
    glEnableVertexAttribArray(v_diffuse);
    DEBUG_GL
    glVertexAttribPointer(v_diffuse, 4, GL_FLOAT, false, sizeof(vertex), offsetof(vertex, diffuse));

    uint32 u_view_matrix = glGetUniformLocation(pr1.program_id, "view_matrix");
    error_if(u_view_matrix < 0, "Bad Uniform Location for view_matrix: %u", u_view_matrix);
    uint32 u_proj_matrix = glGetUniformLocation(pr1.program_id, "proj_matrix");
    error_if(u_proj_matrix < 0, "Bad Uniform Location for proj_matrix: %u", u_proj_matrix);
    uint32 u_model_matrix = glGetUniformLocation(pr1.program_id, "model_matrix");
    error_if(u_model_matrix < 0, "Bad Uniform Location for model_matrix: %u", u_model_matrix);
    uint32 u_ambient_color = glGetUniformLocation(pr1.program_id, "ambient_color");
    error_if(u_ambient_color < 0, "Bad Uniform Location for ambient_color: %u", u_ambient_color);


    DEBUG_GL
    glUniformMatrix4fv(u_proj_matrix, 1, GL_FALSE, cam.projection_matrix._);
    DEBUG_GL
    glUniformMatrix4fv(u_view_matrix, 1, GL_FALSE, cam.view_matrix._);
    DEBUG_GL
    glUniformMatrix4fv(u_model_matrix, 1, GL_FALSE, identity_matrix._);
    DEBUG_GL
    glUniform4fv(u_ambient_color, 1, ambient_color);
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
            //printf("model matrix\n");
            //print_matrix(model_mat2);
            printf("\ncamera proje matrix\n");
            print_matrix(cam.projection_matrix);
            printf("\ncamera view matrix\n");
            print_matrix(cam.view_matrix);

        }

        nextpolation = (phys_accum / phys_step);
        prevpolation = 1 - nextpolation;

        DEBUG_GL
        /*
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
        */
        use_program(&pr1);
        DEBUG_GL
        //gnd_draw(&g);
        DEBUG_GL

        glBindBuffer(GL_ARRAY_BUFFER, tri_vbo_id);
        glBindVertexArray(tri_vao_id);
        //debug("tex->vertices.len %u", tex->vertices.len);
        glDrawArrays(GL_TRIANGLES, 0, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    printf("See you next time!\n");
    return 0;
}
