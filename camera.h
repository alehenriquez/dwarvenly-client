#define CAMERA_H


typedef struct camera {
    float persp_matrix[16];
    float cam_matrix[16];
} camera_t;

typedef struct perspective {
    float fovy;
    float aspect;
    float znear;
    float zfar;
} perspective_t;

typedef struct cam_loc {
    float x;
    float y;
    float z;
    float fx;
    float fy;
    float fz;
    float ux;
    float uy;
    float uz;
} cam_loc_t;


camera_t mk_camera(perspective_t p, cam_loc_t c);
void persp_to_matrix(float *m, float fovy, float aspect, float znear, float zfar);
void cam_to_matrix(float *r, float x, float y, float z, float cx, float cy, float cz, float ux, float uy, float uz);

