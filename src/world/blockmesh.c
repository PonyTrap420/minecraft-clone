#include "blockmesh.h"
#include <stdlib.h>

static const Vec3 cube_vertices[] = {
    {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
    {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
};

static const unsigned int cube_indices[] = {
    0, 1, 2, 2, 3, 0,

    1, 5, 6, 6, 2, 1,

    5, 4, 7, 7, 6, 5,

    4, 0, 3, 3, 7, 4,

    3, 2, 6, 6, 7, 3,

    4, 5, 1, 1, 0, 4
};

static const Vec2 cube_uvs[] = {
    {0, 0}, {1, 0}, {1, 1}, {0, 1},
    {0, 0}, {1, 0}, {1, 1}, {0, 1},
};

BlockMesh* block_mesh_create() {
    BlockMesh* mesh = (BlockMesh*)malloc(sizeof(BlockMesh));
    if (!mesh) return NULL;

    mesh->vertex_count = sizeof(cube_vertices) / sizeof(Vec3);
    mesh->index_count = sizeof(cube_indices) / sizeof(unsigned int);
    mesh->uv_count = sizeof(cube_uvs) / sizeof(Vec2);

    mesh->vertices = (Vec3*)malloc(sizeof(Vec3) * mesh->vertex_count);
    mesh->indices = (unsigned int*)malloc(sizeof(unsigned int) * mesh->index_count);
    mesh->uvs = (Vec2*)malloc(sizeof(Vec2) * mesh->uv_count);

    if (!mesh->vertices || !mesh->indices || !mesh->uvs) {
        block_mesh_destroy(mesh);
        return NULL;
    }

    for (size_t i = 0; i < mesh->vertex_count; ++i) {
        mesh->vertices[i] = cube_vertices[i];
        mesh->uvs[i] = cube_uvs[i];
    }

    for (size_t i = 0; i < mesh->index_count; ++i) {
        mesh->indices[i] = cube_indices[i];
    }

    return mesh;
}

void block_mesh_destroy(BlockMesh* mesh) {
    if (!mesh) return;
    free(mesh->vertices);
    free(mesh->uvs);
    free(mesh->indices);
    free(mesh);
}