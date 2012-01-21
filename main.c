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


static int running = 1;



int main() {
    win_open(1024, 768, 8, 8, 8, 0, 24, 8, 1);
    DEBUG_GL
    printf("w_width %5.3f w_height %5.3f\n", (float)w_width, (float)w_height);
    perspective_t p = {
        .fovy=60.0,
        .aspect=(float)w_width/(float)w_height,
        .znear=1.0,
        .zfar=10.0
    };
    cam_loc_t c_loc = {
        .x =0.0, .y =0.0, .z =1.0,
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

    /* TEST */

    // First simple object
	float vert[9] = {
       -0.3, 0.5,-1.0,
       -0.8,-0.5,-1.0,
        0.2,-0.5,-1.0
	};

	float col[9] = {
	    1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
	};


	// Second simple object
	float vert2[9] = {
	    -0.2, 0.5,-1.0,
	     0.3,-0.5,-1.0,
	     0.8, 0.5,-1.0
    };	// vertex array

    float col2[9] = {
	    1.0, 1.0, 0.0,
        0.0, 1.0, 1.0,
        1.0, 0.0, 1.0
	};

	unsigned short indices2[3] = {
        0, 1, 2
	};

    float model_mat1[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    DEBUG_GL

    printf("v_position %i\n", pr1.attribs.v_position);
    printf("v_color %i\n", pr1.attribs.v_color);

    printf("proj_matrix %i\n", pr1.uniforms.proj_matrix);
    printf("model_matrix %i\n", pr1.uniforms.model_matrix);
    printf("view_matrix %i\n", pr1.uniforms.view_matrix);

    DEBUG_GL
    GLuint m_vaoID[2];
	GLuint m_vboID[3];
    DEBUG_GL
	// Two VAOs allocation
	glGenVertexArrays(2, m_vaoID);
    DEBUG_GL

	// First VAO setup
	glBindVertexArray(m_vaoID[0]);
    DEBUG_GL

	glGenBuffers(2, m_vboID);
    DEBUG_GL

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
    DEBUG_GL
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert, GL_STATIC_DRAW);
    DEBUG_GL
	glVertexAttribPointer(pr1.attribs.v_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    DEBUG_GL
	glEnableVertexAttribArray(pr1.attribs.v_position);
    DEBUG_GL

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
    DEBUG_GL
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), col, GL_STATIC_DRAW);
    DEBUG_GL
	glVertexAttribPointer(pr1.attribs.v_color, 3, GL_FLOAT, GL_FALSE, 0, 0);
    DEBUG_GL
	glEnableVertexAttribArray(pr1.attribs.v_color);
    DEBUG_GL

	// Second VAO setup
	model_t mod = mk_model(&pr1, vert2, 9, col2, 9, indices2, 3);
/*
	glBindVertexArray(m_vaoID[1]);
    DEBUG_GL

	glGenBuffers(1, &m_vboID[2]);
    DEBUG_GL

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
    DEBUG_GL
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert2, GL_STATIC_DRAW);
    DEBUG_GL
	glVertexAttribPointer(in_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);
    DEBUG_GL
	glEnableVertexAttribArray(in_Position);
    DEBUG_GL
*/
	glBindVertexArray(0);

    printf("\nmodel matrix\n");
    print_matrix(model_mat1);
    printf("\npersp matrix\n");
    print_matrix(cam.persp_matrix);
    printf("\ncamera matrix\n");
    print_matrix(cam.cam_matrix);


    //glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe debug
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);

    while (running) {
        DEBUG_GL
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(m_vaoID[0]);		// select first VAO

        DEBUG_GL
        glUniformMatrix4fv(pr1.uniforms.proj_matrix, 1, GL_FALSE, cam.persp_matrix);
        glUniformMatrix4fv(pr1.uniforms.view_matrix, 1, GL_FALSE, cam.cam_matrix);
        glUniformMatrix4fv(pr1.uniforms.model_matrix, 1, GL_FALSE, model_mat1);


        glDrawArrays(GL_TRIANGLES, 0, 3);	// draw first object

        DEBUG_GL
        bind_model(&mod);
        draw_model(&mod);
/*
        glBindVertexArray(m_vaoID[1]);		// select second VAO
        glVertexAttrib3f((GLuint)5, 1.0, 0.0, 0.0); // set constant color attribute
        glDrawArrays(GL_TRIANGLES, 0, 3);	// draw second object
*/
        glBindVertexArray(0);
        glfwSwapBuffers();
    }

/* END TEST */

/*
    vertex_t *verts = malloc(8 * sizeof(vertex_t));
    verts[0].x = -1.0; verts[0].y = 0.0; verts[0].z = 2.0;
    verts[1].x =  1.0; verts[1].y = 0.0; verts[1].z = 2.0;
    verts[2].x = -1.0; verts[2].y = 1.0; verts[2].z = 2.0;
    verts[3].x =  1.0; verts[3].y = 1.0; verts[3].z = 2.0;
    verts[4].x = -1.0; verts[4].y = 0.0; verts[4].z = 3.0;
    verts[5].x =  1.0; verts[5].y = 0.0; verts[5].z = 3.0;
    verts[6].x = -1.0; verts[6].y = 1.0; verts[6].z = 3.0;
    verts[7].x =  1.0; verts[7].y = 1.0; verts[7].z = 3.0;

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

    model_t m = mk_model(verts, 8, testi, 36);
    DEBUG_GL
    graphics_t g = mk_graphics(&pr1, &cam, &m, model_mat1);
    DEBUG_GL

    double curr = glfwGetTime();
    double prev = curr;
    double dt = 0.0;
    double phys_accum = 0.0;
    double phys_step = 0.01;
    double nextpolation = 0.0;
    double prevpolation = 0.0;

    DEBUG_GL




    double debug_accum = 0.0;
    double debug_step = 10.0;

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

        debug_accum += dt;
        if (debug_accum >= debug_step) {
            debug_accum = 0.0;
            printf("\n-----------------------------------\n");
            printf("model matrix\n");
            print_matrix(model_mat1);
            printf("\npersp matrix\n");
            print_matrix(cam.persp_matrix);
            printf("\ncamera matrix\n");
            print_matrix(cam.cam_matrix);

        }

        nextpolation = (phys_accum / phys_step);
        prevpolation = 1 - nextpolation;

        //rotate(model_mat1, 1.0, x_axis);
        //rotate(model_mat1, 0.5, y_axis);
        //rotate(model_mat1, 0.3, z_axis);

        draw_graphics(&g);

        glfwSwapBuffers();
        DEBUG_GL
    }

    glfwCloseWindow();
    glfwTerminate();
*/
    printf("See you next time!\n");
    return 0;
}
