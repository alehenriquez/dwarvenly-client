#ifndef GND_H
#include <ro/gnd.h>
#endif

#ifndef MEM_H
#include <locust/rt.mem.h>
#endif

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

#ifndef UTILS_H
#include <utils.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

/*
// Format of .gnd files

byte[4]  magic             // "GRGN" magic bytes
byte	 version_major
byte     version_minor
int32   width
int32   height
float32  proportion        // proportion ratio usually 10.0
uint32   tex_count         // number of textures used
uint32   tex_path_size     // size of texture Path

textures {
    byte[80]  path         // null terminated path to textures file
}

uint32  lmaps_count        // count of lightmap structures
uint32  lmaps_x_size       // size of lightmap grid X
uint32  lmaps_y_size       // size of lightmap grid Y
uint32  lmaps_cell_size    // number of cells by grid

lmaps  {
    byte[64]   brightness  // 8*8 lightmap alphas
    byte[192]  color_rgb   // 8*8 lightmap colors (RGB)
}

uint32  tiles_countr

tiles {
    float32  u_bl		   // texcoord for bottom left
    float32  u_br          // ''               '' right
    float32  u_tl          // etc.
    float32  u_tr
    float32  v_bl
    float32  v_br
    float32  v_tl
    float32  v_tr
    uint16   tex_id        // index of texture used
    uint16   lmap_id       // index of lightmap used
    byte[4]  diffuse       // tile color (RGBA)
}

cells {					   // width*height cells
    float32 height_bl
    float32 height_br
    float32 height_tl
    float32 height_tr
    uint32  tile_up_id
    uint32  tile_front_id
    uint32  tile_right_id
}
*/
/*
typedef struct {
    byte* path;
    vertex* triangles;
    uint32 triangles_count;
    uint8 path_len;
} gnd_texture;

typedef struct {
    byte* name;
    byte* path;
    gnd_texture* textures;
    gnd_tile* tiles;
    gnd_cell* cells;
    uint32 textures_count;
    uint32 tiles_count;
    uint32 cells_count;
    uint32 width;
    uint32 height;
    float32 proportion;
    byte version_major;
    byte version_minor;
} gnd;
*/

#define EMPTY_MATRIX { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
#define IDENTITY_MATRIX { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

// Type meta information
TYPE_ID_DEF(texcoord2);
META_FIELDS_DEF(texcoord2) = {
    2, {
        {TYPE_ID_OF(float32), sizeof(float32), "u"},
        {TYPE_ID_OF(float32), sizeof(float32), "v"},
    }
};
META_DEF(texcoord2, & META_FIELDS_OF(texcoord2), nil);


TYPE_ID_DEF(color);
META_FIELDS_DEF(color) = {
    4, {
        {TYPE_ID_OF(float32), sizeof(float32), "a"},
        {TYPE_ID_OF(float32), sizeof(float32), "r"},
        {TYPE_ID_OF(float32), sizeof(float32), "g"},
        {TYPE_ID_OF(float32), sizeof(float32), "b"},
    }
};
META_DEF(color, & META_FIELDS_OF(color), nil);


TYPE_ID_DEF(gnd_version);
META_FIELDS_DEF(gnd_version) = {
    2, {
        {TYPE_ID_OF(byte), sizeof(byte), "major"},
        {TYPE_ID_OF(byte), sizeof(byte), "minor"},
    }
};
META_DEF(gnd_version, & META_FIELDS_OF(gnd_version), nil);


typedef struct {
    texcoord2 tl, tr, bl, br;
    uint16 tex_id;
    color  diffuse;
} gnd_tile;
#define gnd_tile__type_id ((type_id)332)
extern TYPE_ID_DECL(gnd_tile);
extern META_DECL(gnd_tile);
TYPE_ID_DEF(gnd_tile);
META_FIELDS_DEF(gnd_tile) = {
    6, {
        {TYPE_ID_OF(texcoord2), sizeof(texcoord2), "tl"},
        {TYPE_ID_OF(texcoord2), sizeof(texcoord2), "tr"},
        {TYPE_ID_OF(texcoord2), sizeof(texcoord2), "bl"},
        {TYPE_ID_OF(texcoord2), sizeof(texcoord2), "br"},
        {TYPE_ID_OF(uint16), sizeof(uint16), "tex_id"},
        {TYPE_ID_OF(color), sizeof(color), "diffuse"},
    }
};
META_DEF(gnd_tile, & META_FIELDS_OF(gnd_tile), nil);


typedef struct {
    float32 h_tl, h_tr, h_bl, h_br;
    uint32  tile_up_id;
    uint32  tile_front_id;
    uint32  tile_right_id;
} gnd_cell;
#define gnd_cell__type_id ((type_id)331)
extern TYPE_ID_DECL(gnd_cell);
extern META_DECL(gnd_cell);
TYPE_ID_DEF(gnd_cell);
META_FIELDS_DEF(gnd_cell) = {
    7, {
        {TYPE_ID_OF(float32), sizeof(float32), "h_tl"},
        {TYPE_ID_OF(float32), sizeof(float32), "h_tr"},
        {TYPE_ID_OF(float32), sizeof(float32), "h_bl"},
        {TYPE_ID_OF(float32), sizeof(float32), "h_br"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "tile_up_id"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "tile_front_id"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "tile_right_id"},
    }
};
META_DEF(gnd_cell, & META_FIELDS_OF(gnd_cell), nil);


TYPE_ID_DEF(vertex);
META_FIELDS_DEF(vertex) = {
    8, {
        {TYPE_ID_OF(texcoord2), sizeof(texcoord2), "tex"},
        {TYPE_ID_OF(color), sizeof(color), "diffuse"},
        {TYPE_ID_OF(float32), sizeof(float32), "nx"},
        {TYPE_ID_OF(float32), sizeof(float32), "ny"},
        {TYPE_ID_OF(float32), sizeof(float32), "nz"},
        {TYPE_ID_OF(float32), sizeof(float32), "x"},
        {TYPE_ID_OF(float32), sizeof(float32), "y"},
        {TYPE_ID_OF(float32), sizeof(float32), "z"},
    }
};
META_DEF(vertex, & META_FIELDS_OF(vertex), nil);


TYPE_ID_DEF(gnd_texture);
META_FIELDS_DEF(gnd_texture) = {
    4, {
        {TYPE_ID_OF(ptr), sizeof(byte*), "path"},
        {TYPE_ID_OF(vector), sizeof(vector), "vertices"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "vbo_id"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "vao_id"},
    }
};
META_DEF(gnd_texture, & META_FIELDS_OF(gnd_texture), nil);


TYPE_ID_DEF(gnd);
META_FIELDS_DEF(gnd) = {
    7, {
        {TYPE_ID_OF(ptr), sizeof(byte*), "name"},
        {TYPE_ID_OF(ptr), sizeof(byte*), "path"},
        {TYPE_ID_OF(vector), sizeof(vector), "textures"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "width"},
        {TYPE_ID_OF(uint32), sizeof(uint32), "height"},
        {TYPE_ID_OF(float32), sizeof(float32), "proportion"},
        {TYPE_ID_OF(gnd_version), sizeof(gnd_version), "version"},


    }
};
META_DEF(gnd, & META_FIELDS_OF(gnd), nil);


// Methods
gnd gnd_load(char const * const path) {
    info("Loading GND file: %s", path);
    gnd g;
    byte magic[5] = {0};
    void* p;

    vector tiles;
    vector cells;

    //debug("1");

    FILE *f = fopen(path, "rb");
    warn_goto_if(f == nil, "Cannot open GND file: %s because %s", path, strerror(errno));

    //debug("2");

    // Parsing meta data
    uint32 tex_paths_len;
    uint32 tex_paths_count;
    int32 n = fscanf(f, "%4c%1c%1c%4c%4c%4c%4c%4c", &magic, &g.version.major, &g.version.minor, &g.width, &g.height, &g.proportion, &tex_paths_count, &tex_paths_len);
    warn_goto_if(8 != n, "Invalid GND file: %s at %d; magic: %s , version: %hhu %hhu , width: %u height: %u , proportion: %f tex_paths_count: %u tex_paths_len: %u", path, 8-n, magic, g.version.major, g.version.minor, g.width, g.height, g.proportion, tex_paths_count, tex_paths_len);
    warn_if(strncmp(magic, "GRGN", 4) != 0, "Invalid GND file: expected magic GRGN at start, got %s", magic);

    //debug("3");
    vector_init(&g.textures, & META_OF(gnd_texture), tex_paths_count);
    g.textures.len = tex_paths_count;

    uint32 total = 0;
    int64 ffwd = 0;

    gnd_texture *t = g.textures.items;

    //debug("4");
    // prepare format string for the fscanf of each tex_path
    // -- mainly to limit the reading to the specified tex_path_len
#define BUF_LEN 16
    char tmp[BUF_LEN] = {0};
    snprintf(&tmp[0], BUF_LEN, "%%%uc\0", tex_paths_len);
    // parse all the texture paths into tex->path
    // and make sure that the tex->vertices gets initialized
    for (uint32 i=0, count=0; i < tex_paths_count; ++i) {
        t->path = mem_get(tex_paths_len);
        warn_goto_if(t->path == nil, "tex->path mem_get() error;");
        n = fscanf(f, &tmp[0], t->path);
        warn_goto_if(n != 1, "Invalid GND file: %s at tex_paths + %d", t->path, i);
        vector_init(&t->vertices, & META_OF(vertex));
        t += 1;
    }

    //debug("5");
    // Get ready to parse lightmap meta info in order to skip the whole damn section.
    uint32 lmap_count, lmap_w, lmap_h, lmap_cell_size;
    n = fscanf(f, "%4c%4c%4c%4c", &lmap_count, &lmap_w, &lmap_h, &lmap_cell_size);
    warn_goto_if(n != 4, "Invalid GND file: %s at lightmaps_meta + %d", path, 4-n);
    fseek(f, lmap_count * 4 * lmap_w * lmap_h, SEEK_CUR); // Skip lightmap info. Shaders plz.

    //debug("6");
    // Set up gnd_tiles temporary store to parse into
    uint32 gnd_tiles_count;
    n = fscanf(f, "%4c", &gnd_tiles_count);
    warn_goto_if(n != 1, "Invalid GND file: %s at tiles_count", path);

    vector_init(&tiles, & META_OF(gnd_tile), gnd_tiles_count);
    gnd_tile tile;

    //debug("7");
    byte _r, _g, _b, _a;
    uint32 id;
    for (uint32 i=0; i < gnd_tiles_count; ++i) {
        n = fscanf(f, "%4c%4c%4c%4c%4c%4c%4c%4c%2c%*2c%c%c%c%c",
                  &tile.bl.u, &tile.br.u, &tile.tl.u, &tile.tr.u,
                  &tile.bl.v, &tile.br.v, &tile.tl.v, &tile.tr.v,
                  &tile.tex_id, &_r, &_g, &_b, &_a);
        warn_goto_if(n != 13, "Invalid GND file: %s at tile %d", path, i);
        if ((id = tile.tex_id) > tex_paths_count) {
            warn("Invalid GND file: %s; gnd_tile %u refers to invalid tex_id %u", path, i, id);
        }
        // Convert color from byte[4] to 0.0<-->1.0 float32[4]
        tile.diffuse.r = ((float32)_r) / 255.0;
        tile.diffuse.g = ((float32)_g) / 255.0;
        tile.diffuse.b = ((float32)_b) / 255.0;
        tile.diffuse.a = ((float32)_a) / 255.0;
        vector_push(&tiles, & META_OF(gnd_tile), &tile);
    }

    //debug("8");

    // Set up gnd_cell temporary store to parse into
    uint32 gnd_cells_count = g.width * g.height;
    vector_init(&cells, & META_OF(gnd_cell), gnd_cells_count);
    gnd_cell cell;
    
    for (uint32 i=0; i<gnd_cells_count; ++i) {
        n = fscanf(f, "%4c%4c%4c%4c%4c%4c%4c",
                  &cell.h_bl, &cell.h_br, &cell.h_tl, &cell.h_tr,
                  &cell.tile_front_id, &cell.tile_up_id, &cell.tile_right_id);
        warn_goto_if(n != 7, "Invalid GND file: %s at cell %d", path, i);
        vector_push(&cells, & META_OF(gnd_cell), &cell);
    }

    // Further computation is in memory.
    fclose(f);

    //debug("9");
    gnd_cell curr, right, up;
    vertex v;
    uint32 x, y;

    gnd_tile *tile_curr, *tile_right, *tile_up;
    gnd_texture *tex, *tex_right, *tex_up;

    for (uint32 i=0; i<gnd_cells_count; ++i) {
        x = i % g.width;
        y = i / g.width;
        // front cell
        vector_get(&cells, i, & META_OF(gnd_cell), &curr);
        id = curr.tile_front_id;
        //debug("tile id %u's .. front %d up %d right %d", i, curr.tile_front_id, curr.tile_up_id, curr.tile_right_id);

/*        if (id == (uint32) -1) {
            warn_if((curr.tile_up_id != (uint32)-1) || (curr.tile_right_id != (uint32)-1), "gnd_cell %u's tile_front_id is missing (-1) but has up (%u) or right (%u)", i, curr.tile_up_id, curr.tile_right_id);
            continue;
        }
*/
        // front cell
        if ((id = curr.tile_front_id) < ((uint32)-1)) {
            if (id > gnd_tiles_count) {
                warn("gnd_cell %u's tile_front_id refers to invalid tile id %d", i, id);
            } else {
                tile_curr = &(((gnd_tile*)tiles.items)[id]);
                //debug("1 tile_curr: %p", tile_curr);
                tex = g.textures.items+(tile_curr->tex_id*sizeof(gnd_texture));
                //debug("2 tex: %p, tex->vertices %p", tex, tex->vertices);
                // First triangle
                v.x = x;
                v.y = y;
                v.z = 650.0+curr.h_bl;
                v.tex = tile_curr->bl;
                v.diffuse = tile_curr->diffuse;
                //debug("3 v.x %f v.y %f v.z %f v.tex.u %f v.tex.v %f v.diffuse.a %hhu v.diffuse.r %hhu v.diffuse.g %hhu v.diffuse.b %hhu", v.x, v.y, v.z, v.tex.u, v.tex.v, v.diffuse.a, v.diffuse.r, v.diffuse.g, v.diffuse.b);
                vector_push(&tex->vertices, & META_OF(vertex), &v);
                v.x = x+1.0;
                v.y = y;
                v.z = 650.0+curr.h_br;
                v.tex = tile_curr->br;
                //debug("4");
                vector_push(&tex->vertices, & META_OF(vertex), &v);
                v.x = x+1.0;
                v.y = y+1.0;
                v.z = 650.0+curr.h_tr;
                v.tex = tile_curr->tr;
                //debug("5");
                vector_push(&tex->vertices, & META_OF(vertex), &v);

                // Second triangle
                // unchanged because it's the 3rd vertex above again. Quads -> Triangles conversion.
                //debug("6");
                vector_push(&tex->vertices, & META_OF(vertex), &v);
                v.x = x;
                v.y = y+1.0;
                v.z = 650.0+curr.h_tl;
                v.tex = tile_curr->tl;
                //debug("7");
                vector_push(&tex->vertices, & META_OF(vertex), &v);
                v.x = x;
                v.y = y;
                v.z = 650.0+curr.h_bl;
                v.tex = tile_curr->bl;
                //debug("8");
                vector_push(&tex->vertices, & META_OF(vertex), &v);
            }
        }

        // right cell
        if ((x < g.width-1) && ((id = cell.tile_right_id) < (uint32)-1)) {
            if (id > gnd_tiles_count) {
                warn("gnd_cell %u 's tile_right_id refers to invalid tile id %u", i, id);
            } else {
                vector_get(&cells, i+1, & META_OF(gnd_cell), &right);
                tile_right = &(((gnd_tile*)tiles.items)[id]);
                tex_right = g.textures.items+(tile_right->tex_id*sizeof(gnd_texture));
                // TODO: This diffuse interpolation may not be official behavior.
                // tri 1
                // curr.br
                v.x = x+1.0;
                v.y = y;
                v.z = 650.0+curr.h_br;
                v.tex = tile_curr->br;
                v.diffuse = tile_curr->diffuse;
                vector_push(&tex_right->vertices, & META_OF(vertex), &v);
                // right.bl
                v.x = x+1.0;
                v.y = y;
                v.z = 650.0+right.h_bl;
                v.tex = tile_right->bl;
                v.diffuse = tile_right->diffuse;
                vector_push(&tex_right->vertices, & META_OF(vertex), &v);
                // right.tl
                v.x = x+1.0;
                v.y = y+1.0;
                v.z = 650.0+right.h_tl;
                v.tex = tile_right->tl;
                vector_push(&tex_right->vertices, & META_OF(vertex), &v);
                // tri 2
                vector_push(&tex_right->vertices, & META_OF(vertex), &v);
                // curr.tr
                v.x = x+1.0;
                v.y = y;
                v.z = 650.0+curr.h_tr;
                v.tex = tile_curr->tr;
                v.diffuse = tile_curr->diffuse;
                vector_push(&tex_right->vertices, & META_OF(vertex), &v);
                // curr.br
                v.x = x+1.0;
                v.y = y;
                v.z = 650.0+curr.h_br;
                v.tex = tile_curr->br;
                v.diffuse = tile_curr->diffuse;
                vector_push(&tex_right->vertices, & META_OF(vertex), &v);
            }
        }

        // up cell
        
        if ((y < g.height-1) && ((id = cell.tile_up_id) < (uint32)-1)) {
            if (id > gnd_tiles_count) {
                warn("gnd_cell %u 's tile_up_id refers to invalid tile id %u", i, id);
            } else {
                vector_get(&cells, i+g.width, & META_OF(gnd_cell), &up);
                tile_up = &(((gnd_tile*)tiles.items)[id]);
                tex_up = g.textures.items+(tile_up->tex_id*sizeof(gnd_texture));
                // TODO: This diffuse interpolation may not be official behavior.
                // tri 1
                // curr.tl
                v.x = x;
                v.y = y+1.0;
                v.z = 650.0+curr.h_tl;
                v.tex = tile_curr->tl;
                v.diffuse = tile_curr->diffuse;
                vector_push(&tex_up->vertices, & META_OF(vertex), &v);
                // curr.tr
                v.x = x+1.0;
                v.y = y+1.0;
                v.z = 650.0+curr.h_tr;
                v.tex = tile_curr->tr;
                vector_push(&tex_up->vertices, & META_OF(vertex), &v);
                // up.br
                v.x = x+1.0;
                v.y = y+1.0;
                v.z = 650.0+up.h_br;
                v.tex = tile_up->br;
                v.diffuse = tile_up->diffuse;
                vector_push(&tex_up->vertices, & META_OF(vertex), &v);
                // tri 2
                vector_push(&tex_up->vertices, & META_OF(vertex), &v);
                // up.bl
                v.x = x;
                v.y = y+1.0;
                v.z = 650.0+up.h_bl;
                v.tex = tile_up->bl;
                vector_push(&tex_up->vertices, & META_OF(vertex), &v);
                // curr.tl
                v.x = x;
                v.y = y+1.0;
                v.z = 650.0+curr.h_tl;
                v.tex = tile_curr->tl;
                v.diffuse = tile_curr->diffuse;
                vector_push(&tex_up->vertices, & META_OF(vertex), &v);
            }
        }
    }
    
    vector_deinit(&cells);
    vector_deinit(&tiles);
    info("Loaded GND file: %s", path);
  warn:
  error:
    return g;
}

void nonnullfunc gnd_load_gl(gnd* self, program_t *p, camera *c, GLfloat *ambient_color) {
    self->camera = c;
    self->ambient_color[0] = ambient_color[0];
    self->ambient_color[1] = ambient_color[1];
    self->ambient_color[2] = ambient_color[2];
    self->ambient_color[3] = ambient_color[3];

    self->u_view_matrix = glGetUniformLocation(p->program_id, "view_matrix");
    error_if(self->u_view_matrix < 0, "Bad Uniform Location for view_matrix: %u", self->u_view_matrix);
    self->u_proj_matrix = glGetUniformLocation(p->program_id, "proj_matrix");
    error_if(self->u_proj_matrix < 0, "Bad Uniform Location for proj_matrix: %u", self->u_proj_matrix);
    self->u_model_matrix = glGetUniformLocation(p->program_id, "model_matrix");
    error_if(self->u_model_matrix < 0, "Bad Uniform Location for model_matrix: %u", self->u_model_matrix);
    self->u_ambient_color = glGetUniformLocation(p->program_id, "ambient_color");
    error_if(self->u_ambient_color < 0, "Bad Uniform Location for ambient_color: %u", self->u_ambient_color);


    self->a_position = glGetAttribLocation(p->program_id, "v_position");
    assert(self->a_position >= 0);
    DEBUG_GL
    self->a_diffuse = glGetAttribLocation(p->program_id, "v_diffuse");
    assert(self->a_diffuse >= 0);
    DEBUG_GL

    self->model_matrix = identity_matrix;

    // Allocate vbo and vao for all the textures' vertices
    gnd_texture *tex = self->textures.items;
    DEBUG_GL
    for (uint32 i=self->textures.len; i > 0; --i) {
        glGenBuffers(1, &tex->vbo_id);
        DEBUG_GL
        glBindBuffer(GL_ARRAY_BUFFER, tex->vbo_id);
        DEBUG_GL
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*tex->vertices.len, tex->vertices.items, GL_STATIC_DRAW);
        DEBUG_GL
        
        glGenVertexArrays(1, &tex->vao_id);
        DEBUG_GL
        glBindVertexArray(tex->vao_id);
        DEBUG_GL

        glEnableVertexAttribArray(self->a_position);
        DEBUG_GL
        glVertexAttribPointer(self->a_position, 3, GL_FLOAT, false, sizeof(vertex), 0);
        DEBUG_GL
        glEnableVertexAttribArray(self->a_diffuse);
        DEBUG_GL
        glVertexAttribPointer(self->a_diffuse, 4, GL_FLOAT, false, sizeof(vertex), offsetof(vertex, diffuse));
        DEBUG_GL

        tex += 1;
    }
}

void nonnullfunc gnd_draw(gnd* self) {
    DEBUG_GL
    glUniformMatrix4fv(self->u_proj_matrix, 1, GL_FALSE, self->camera->projection_matrix._);
    DEBUG_GL
    glUniformMatrix4fv(self->u_view_matrix, 1, GL_FALSE, self->camera->view_matrix._);
    DEBUG_GL
    glUniformMatrix4fv(self->u_model_matrix, 1, GL_FALSE, self->model_matrix._);
    DEBUG_GL
    glUniform4fv(self->u_ambient_color, 1, self->ambient_color);
    DEBUG_GL

    gnd_texture* tex = self->textures.items;
    uint64 i = self->textures.len;
    while (i-->0) {
        if (tex->vertices.len > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, tex->vbo_id);
            glBindVertexArray(tex->vao_id);
            //debug("tex->vertices.len %u", tex->vertices.len);
            glDrawArrays(GL_TRIANGLES, 0, tex->vertices.len / 3);
        }

        tex += 1;
    }
}
