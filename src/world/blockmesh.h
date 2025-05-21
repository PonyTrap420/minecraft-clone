#pragma once

#include <stddef.h>
#include <cglm/cglm.h>

#include "block/blocktypes.h"
#include "../gfx/atlas.h"

#define X(name, value, tex) name = value,
typedef enum {
    BLOCK_LIST
    BLOCK_COUNT
} BlockType;
#undef X

typedef struct {
    vec3* vertices;
    vec2* uvs;
    unsigned int* indices;

    size_t vertex_count;
    size_t uv_count;
    size_t index_count;
} BlockMesh;

BlockMesh* block_mesh_create();
void block_mesh_destroy(BlockMesh* mesh);
int add_face(float* out, int x, int y, int z, int face, BlockType block_type);