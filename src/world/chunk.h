#pragma once

#include <inttypes.h>
#include "../gfx/vao.h"
#include <cglm/cglm.h>
#include "blockmesh.h"
#include "../gfx/shader.h"
#include "../gfx/renderer.h"

typedef struct World World;

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16

#define MAX_VERTS (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 36)
typedef struct {
    World* world;

    uint8_t blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    
    VAO vao;
    int vertex_count;

    int chunk_x;
    int chunk_z; 

    float* vertex_data; 
} Chunk;

Chunk* chunk_init(World* world, int coordX, int coordY);
void chunk_prepare(Chunk* chunk);
void chunk_render(Chunk* chunk, Shader* shader);
void chunk_generate_mesh(Chunk* chunk);
int is_face_visible(World* world, Chunk* chunk, int x, int y, int z, int dx, int dy, int dz);

void chunk_free(Chunk* chunk);