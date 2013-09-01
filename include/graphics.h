#define GRAPHICS_H

#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#ifndef SHADERS_H
#include "shaders.h"
#endif

#ifndef MODELS_H
#include "models.h"
#endif

#include <math.h>

#define EMPTY_MATRIX { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
#define IDENTITY_MATRIX { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

typedef union {
    struct {
        float32 x, y;
    };
    float32 _[2];
} vec2;

typedef union {
    struct {
        float32 x, y, z;
    };
    float32 _[3];
} vec3;

typedef union {
    struct {
        float32 x, y, z, w;
    };
    float32 _[4];
} vec4;

typedef union {
    struct {
        float32 x1, y1, z1, x2, y2, z2, x3, y3, z3;
    };
    float32 _[9];
    float32 as_multi[3][3];
    vec3 as_vec3[3];
} mat3;

typedef union {
    struct {
        float32 x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4;
    };
    float32 _[16];
    float32 as_multi[4][4];
    vec4 as_vec4[4];
} mat4;

typedef struct {
    float32 fovy;
    float32 aspect;
    float32 znear;
    float32 zfar;
} perspective;

typedef struct {
    mat4 projection_matrix;
    mat4 view_matrix;

    vec3 pos;
    vec3 fwd;
    vec3 up;

    perspective p;
} camera;

typedef struct transformation {
    mat4 proj_matrix;
    mat4 model_matrix;
    mat4 view_matrix;
} transformation_t;

typedef struct graphics {
    // GPU side data
    GLuint ubo_id;
    GLuint ubo_loc;

    // CPU side data
    model_t *model;
    float *model_matrix;

    transformation_t trans;
    program_t *program;
    camera *camera;

	GLfloat ambient_color[4];
	GLfloat diffuse_color[4];
	GLfloat specular_color[4];

	GLfloat light_pos[3];
} graphics_t;

typedef enum {
    x_axis,
    y_axis,
    z_axis
} axis;

vec3
static inline purestfunc
cross(vec3 v1, vec3 v2) {
    vec3 ret;
    ret.x = v1.y*v2.z - v1.z*v2.y;
    ret.y = v1.z*v2.x - v1.x*v2.z;
    ret.z = v1.x*v2.y - v1.y*v2.x;
    return ret;
}

float32
static inline overloaded purestfunc
fsqrt(float32 n) {
    int32 i;
    float32 x2;
    const float32 threehalfs = 1.5F;

    x2 = n * 0.5F;
    i  = (int32)n;                       // cast the floating point number number into an integer
    i  = 0x5f375a86 - ( i >> 1 );
    n  = (float32)i;
    n  = n * (threehalfs - (x2 * n * n));   // 1st iteration
    n  = n * (threehalfs - (x2 * n * n));   // 2nd iteration, this can be removed for less accuracy and more speed
    n  = n * (threehalfs - (x2 * n * n));   // 3rd iteration, this can be removed for less accuracy and more speed

    return 1.0f/n;
}

/*
float64
static inline overloaded purestfunc
isqrt(float64 n) {
    int64 i;
    float64 x2;
    const float64 threehalfs = 1.5;

    x2 = n * 0.5;
    i  = (int64)n;                       // cast the floating point number number into an integer
    i  = 0x5fe6eb50c7b537a9 - ( i >> 1 );
    n  = (float64)i;
    n  = n * (threehalfs - (x2 * n * n));   // 1st iteration
    //n  = n * (threehalfs - (x2 * n * n));   // 2nd iteration, this can be removed for less accuracy and more speed

    return n;
}
*/

vec3
static inline purestfunc
sub(vec3 v1, vec3 v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

float32
static inline purestfunc
dot(vec3 v1, vec3 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z+v2.z;
}

float32
static inline purestfunc
vlen(vec3 v) {
    return fsqrt(dot(v, v));
}

vec3
static inline purestfunc
normalize(vec3 v) {
    float32 len = vlen(v);
    if (len != 0.0f) {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}

mat4 
static inline overloaded purestfunc
mul(mat4 m1, mat4 m2) {
    mat4 ret = EMPTY_MATRIX;
    uint32 i,j,k;
    for (i=0; i<4; ++i) {
        for (j=0; j<4; ++j) {
            for (k=0; k<4; ++k) {
                ret._[i*4+j] += m1._[i*4+k] * m2._[k*4+j];
            }
        }
    }
    return ret;
}

vec3
static inline overloaded purestfunc
mul(vec3 v1, vec3 v2) {
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    return v1;
}

vec3
static inline overloaded purestfunc
mul(vec3 v1, float32 f) {
    v1.x *= f;
    v1.y *= f;
    v1.z *= f;
    return v1;
}

mat4
static inline purestfunc
translate(mat4 m, vec3 v) {
    mat4 m2 = IDENTITY_MATRIX;
    m2._[12] = v.x;
    m2._[13] = v.y;
    m2._[14] = v.z;

    return mul(m, m2);
}

#define PI_DIV_180 0.0174532925199f

mat4
static inline purestfunc
rotate(mat4 m, float32 angle, axis ax) {
    mat4 ret = IDENTITY_MATRIX;
    const int8 cos1[3] = { 5, 0, 0 };
    const int8 cos2[3] = { 10, 10, 5 };
    const int8 sin1[3] = { 6, 2, 1 };
    const int8 sin2[3] = { 9, 8, 4 };

    ret._[cos1[ax]] = (float32) cos(PI_DIV_180 * angle);
    ret._[sin1[ax]] = (float32)-sin(PI_DIV_180 * angle);
    ret._[sin2[ax]] =-ret._[sin1[ax]];
    ret._[cos2[ax]] = ret._[cos1[ax]];

    return mul(m, ret);
}



mat4
static inline purestfunc
mk_view_matrix(vec3 eye, vec3 center, vec3 up) {
    mat4 ret = IDENTITY_MATRIX;

    vec3 fwd = normalize(sub(eye, center));
    vec3 side = normalize(cross(fwd, up));
    up = cross(fwd, side);

    ret._[0] = side.x;
    ret._[4] = side.y;
    ret._[8] = side.z;

    ret._[1] = up.x;
    ret._[5] = up.y;
    ret._[9] = up.z;

    ret._[2] = -fwd.x;
    ret._[6] = -fwd.y;
    ret._[10] = -fwd.z;

    ret._[12] = -dot(eye, side);
    ret._[13] = -dot(eye, up);
    ret._[14] = -dot(eye, fwd);

    ret._[15] = 1.0f;

    return ret;
}

mat4
static inline overloaded purestfunc
mk_projection_matrix(float32 fovy, float32 aspect, float32 znear, float32 zfar) {
    mat4 ret = EMPTY_MATRIX;
    float32 ymax = tan((fovy * 0.5f) * (M_PI / 180.0f)) * znear;
    float32 ymin =-ymax;
    float32 xmax = ymax * aspect;
    float32 xmin =-xmax;

    float32 t = 2.0f * znear;
    float32 t2 = xmax - xmin;
    float32 t3 = ymax - ymin;
    float32 t4 = zfar - znear;

    ret._[0] = t / t2;
    ret._[5] = t / t3;
    ret._[8] = (xmax + xmin) / t2;
    ret._[9] = (ymax + ymin) / t3;
    ret._[10] = -(zfar + znear) / t4;
    ret._[11] = -1.0f;
    ret._[14] = (-t * zfar) / t4;

    return ret;
}

mat4
static inline overloaded purestfunc
mk_projection_matrix(perspective p) {
    return mk_projection_matrix(p.fovy, p.aspect, p.znear, p.zfar);
}

extern const mat4 identity_matrix;
extern const mat4 empty_matrix;


void print_matrix(mat4 m);
graphics_t mk_graphics(program_t *p, camera *c, model_t *m, float *model_mat, GLfloat *ambient_color, GLfloat *diffuse_color, GLfloat *specular_color, GLfloat *light_pos);
void bind_graphics(graphics_t *g);
void rm_graphics(graphics_t *g);
void draw_graphics(graphics_t *g);
