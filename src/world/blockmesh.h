#pragma once

#include <stddef.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float u, v;
} Vec2;

typedef struct {
    Vec3* vertices;
    Vec2* uvs;
    unsigned int* indices;

    size_t vertex_count;
    size_t uv_count;
    size_t index_count;
} BlockMesh;

BlockMesh* block_mesh_create();
void block_mesh_destroy(BlockMesh* mesh);