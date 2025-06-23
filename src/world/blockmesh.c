#include "blockmesh.h"
#include <stdlib.h>

#define X(name, value, tex) tex,
const char* block_textures[] = {
    BLOCK_LIST
};
#undef X

static vec3 cube_vertices[] = {
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

static vec2 cube_uvs[] = {
    {0, 0}, {1, 0}, {1, 1}, {0, 1},
    {0, 0}, {1, 0}, {1, 1}, {0, 1},
};

BlockMesh* block_mesh_create() {
    BlockMesh* mesh = (BlockMesh*)malloc(sizeof(BlockMesh));
    if (!mesh) return NULL;

    mesh->vertex_count = sizeof(cube_vertices) / sizeof(vec3);
    mesh->index_count = sizeof(cube_indices) / sizeof(unsigned int);
    mesh->uv_count = sizeof(cube_uvs) / sizeof(vec2);

    mesh->vertices = (vec3*)malloc(sizeof(vec3) * mesh->vertex_count);
    mesh->indices = (unsigned int*)malloc(sizeof(unsigned int) * mesh->index_count);
    mesh->uvs = (vec2*)malloc(sizeof(vec2) * mesh->uv_count);

    if (!mesh->vertices || !mesh->indices || !mesh->uvs) {
        block_mesh_destroy(mesh);
        return NULL;
    }

    for (size_t i = 0; i < mesh->vertex_count; ++i) {
        glm_vec3_copy(cube_vertices[i], mesh->vertices[i]);
        glm_vec2_copy(cube_uvs[i], mesh->uvs[i]);
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

int add_face(float* out, int x, int y, int z, int face, BlockType block_type) {
    vec3 base_vertices[] = {
        {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}, // 0-3: front face
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, // 4-7: back face
    };

    const unsigned char face_indices[6][4] = {
        {4, 5, 6, 7}, // back (-Z)
        {0, 1, 2, 3}, // front (+Z)
        {4, 0, 3, 7}, // left (-X)
        {1, 5, 6, 2}, // right (+X)
        {4, 5, 1, 0}, // bottom (-Y)
        {3, 2, 6, 7}, // top (+Y)
    };

    // Compute world-space vertices
    vec3 world_vertices[8];
    for (int i = 0; i < 8; ++i) {
        glm_vec3_copy(base_vertices[i], world_vertices[i]);
        world_vertices[i][0] += (float)x;
        world_vertices[i][1] += (float)y;
        world_vertices[i][2] += (float)z;
    }

    const unsigned char* idx = face_indices[face];

    // Get top-left UV from atlas
    const char* tex_name = block_textures[block_type];
    vec2 uv_top_left = {0.0f, 0.0f};
    if (tex_name) {
        atlas_getuv(tex_name, uv_top_left);
    }

    float cell_uv = 1.0f / 43.0f; // assuming 16x16 grid
    vec2 uvs[4] = {
        {uv_top_left[0],           uv_top_left[1] + cell_uv},  // bottom-left
        {uv_top_left[0] + cell_uv, uv_top_left[1] + cell_uv},  // bottom-right
        {uv_top_left[0] + cell_uv, uv_top_left[1]},            // top-right
        {uv_top_left[0],           uv_top_left[1]}             // top-left
    };

    // We write 6 vertices (two triangles), each vertex 6 floats: 3 pos + 2 uv + 1 face index
    // Triangle 1: vertices idx[0], idx[1], idx[2]
    for (int i = 0; i < 3; ++i) {
        int vi = i; // vertex index in this triangle
        vec3* pos = &world_vertices[idx[vi]];
        vec2* uv = &uvs[vi];

        out[i*6 + 0] = (*pos)[0];
        out[i*6 + 1] = (*pos)[1];
        out[i*6 + 2] = (*pos)[2];
        out[i*6 + 3] = (*uv)[0];
        out[i*6 + 4] = 1.0f-(*uv)[1];
        out[i*6 + 5] = (float)face; // new: face index
    }

    // Triangle 2: vertices idx[0], idx[2], idx[3]
    int second_tri_indices[3] = {0, 2, 3};
    for (int i = 0; i < 3; ++i) {
        int vi = second_tri_indices[i];
        vec3* pos = &world_vertices[idx[vi]];
        vec2* uv = &uvs[vi];

        out[(3+i)*6 + 0] = (*pos)[0];
        out[(3+i)*6 + 1] = (*pos)[1];
        out[(3+i)*6 + 2] = (*pos)[2];
        out[(3+i)*6 + 3] = (*uv)[0];
        out[(3+i)*6 + 4] = 1.0f - (*uv)[1];
        out[(3+i)*6 + 5] = (float)face; // new: face index
    }

    return 6; // 6 vertices added
}