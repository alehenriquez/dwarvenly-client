#include <string.h>
#include <math.h>
#include <GL3/gl3w.h>

#ifndef CAMERA_H
#include "camera.h"
#endif

#ifndef GRAPHICS_H
#include "graphics.h"
#endif

#define PI_DIV_360 0.00872664625

camera_t mk_camera(perspective_t p, cam_loc_t c) {
    camera_t cam;
    persp_to_matrix(cam.persp_matrix, p.fovy, p.aspect, p.znear, p.zfar);
    cam_to_matrix(cam.cam_matrix, c.x, c.y, c.z, c.fx, c.fy, c.fz, c.ux, c.uy, c.uz);
    return cam;
}

void persp_to_matrix(float *m, float fovy, float aspect, float znear, float zfar) {

    float ymax = tan(fovy * PI_DIV_360) * znear;
    float ymin =-ymax;
    float xmax = ymax * aspect;
    float xmin =-xmax;

    float t = 2.0 * znear;
    float t2 = xmax - xmin;
    float t3 = ymax - ymin;
    float t4 = zfar - znear;

    m[0] = t / t2;
    m[1] = 0.0;
    m[2] = 0.0;
    m[3] = 0.0;
    m[4] = 0.0;
    m[5] = t / t3;
    m[6] = 0.0;
    m[7] = 0.0;
    m[8] = (xmax + xmin) / t2;
    m[9] = (ymax + ymin) / t3;
    m[10] = (-zfar - znear) / t4;
    m[11] = -1.0;
    m[12] = 0.0;
    m[13] = 0.0;
    m[14] = (-t * zfar) / t4;
    m[15] = 0.0;

    return;
}

void cam_to_matrix(float *r, float x, float y, float z, float cx, float cy, float cz, float ux, float uy, float uz) {
    memset(r, 0.0, sizeof(GLfloat) * 16);

    // calculating the viewing vector
    float fwd[3] = {cx-x, cy-y, cz-z};

    float fwd_mag = sqrt(fwd[0]*fwd[0] + fwd[1]*fwd[1] + fwd[2]*fwd[2]);
    float up_mag = sqrt(ux*ux + uy*uy + uz*uz);

    // normalizing the viewing vector
    if( fwd_mag != 1) {
        fwd[0] = fwd[0]/fwd_mag;
        fwd[1] = fwd[1]/fwd_mag;
        fwd[2] = fwd[2]/fwd_mag;
    }

    // normalising the up vector.
    if( up_mag != 1 ) {
        ux = ux/up_mag;
        uy = uy/up_mag;
        uz = uz/up_mag;
    }

    float side[3], up[3];

    cross(side, fwd[0], fwd[1], fwd[2], ux, uy, uz);
    cross(up, side[0], side[1], side[2], fwd[0], fwd[1], fwd[2]);


    r[0] = side[0];
    r[4] = side[1];
    r[8] = side[2];

    r[1] = up[0];
    r[5] = up[1];
    r[9] = up[2];

    r[2] =-fwd[0];
    r[6] =-fwd[1];
    r[10]=-fwd[2];

    r[15]=1.0;

    translate(r, -x, -y, -z);
}
