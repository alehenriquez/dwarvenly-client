#ifndef GND_H
#define GND_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef VECTOR_H
#include <locust/ds.vector.h>
#endif

#ifndef SHADERS_H
#include <shaders.h>
#endif

#ifndef GRAPHICS_H
#include <graphics.h>
#endif

typedef struct {
    float32 a, r, g, b;
} color;
#define color__type_id ((type_id)333)
extern TYPE_ID_DECL(color);
extern META_DECL(color);

typedef struct {
    float32 u, v;
} texcoord2;
#define texcoord2__type_id ((type_id)334)
extern TYPE_ID_DECL(texcoord2);
extern META_DECL(texcoord2);

typedef struct {
    byte major, minor;
} gnd_version;
#define gnd_version__type_id ((type_id)335)
extern TYPE_ID_DECL(gnd_version);
extern META_DECL(gnd_version);

typedef struct {
    texcoord2 tex;
    color diffuse;
    float32 nx, ny, nz;
    float32 x, y, z;
} vertex;
#define vertex__type_id ((type_id)336)
extern TYPE_ID_DECL(vertex);
extern META_DECL(vertex);

typedef struct {
    byte* path;
    vector vertices;
    uint32 vbo_id;
    uint32 vao_id;
} gnd_texture;
#define gnd_texture__type_id ((type_id)337)
extern TYPE_ID_DECL(gnd_texture);
extern META_DECL(gnd_texture);

typedef struct {
    byte* name;
    byte* path;
    vector textures;
    uint32 width;
    uint32 height;
    float32 proportion;
    gnd_version version;

    // graphics related
    program_t *p;
    camera *camera;
    float32 ambient_color[4];
    mat4 model_matrix;

    // shader locations
    uint32 u_view_matrix;
    uint32 u_proj_matrix;
    uint32 u_model_matrix;
    uint32 u_ambient_color;
    uint32 a_position;
    uint32 a_diffuse;
} gnd;
#define gnd__type_id ((type_id)338)
extern TYPE_ID_DECL(gnd);
extern META_DECL(gnd);

gnd gnd_load(char const * const path);
void gnd_unload(gnd*);
void gnd_draw(gnd*);
void gnd_debug(gnd*);
void nonnullfunc gnd_draw(gnd* self);
void nonnullfunc gnd_load_gl(gnd* self, program_t *p, camera *c, float32 *ambient_color);

#endif // GND_H