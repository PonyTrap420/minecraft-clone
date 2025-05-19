#include "chunk.h"
#include <cglm/cglm.h>

void chunk_generate_mesh(Chunk* chunk) {
    chunk->vertices = malloc(sizeof(vec3) * MAX_VERTS);
    if (!chunk->vertices) {
        fprintf(stderr, "Failed to allocate mesh buffer\n");
        exit(1);
    }
    chunk->vertex_count = 0;

    for (int x = 0; x < CHUNK_SIZE_X; x++) {
        for (int y = 0; y < CHUNK_SIZE_Y; y++) {
            for (int z = 0; z < CHUNK_SIZE_Z; z++) {
                if (chunk->blocks[x][y][z] == 0) continue;
                
                int directions[6][3] = {
                    { 0,  0, -1}, // -Z (back)
                    { 0,  0,  1}, // +Z (front)
                    {-1,  0,  0}, // -X (left)
                    { 1,  0,  0}, // +X (right)
                    { 0, -1,  0}, // -Y (bottom)
                    { 0,  1,  0}, // +Y (top)
                };
                
              
                for (int i = 0; i < 6; i++) {
                    int dx = directions[i][0];
                    int dy = directions[i][1];
                    int dz = directions[i][2];
                    if (is_face_visible(chunk, x, y, z, dx, dy, dz)) {
                        chunk->vertex_count += add_face(chunk->vertices + chunk->vertex_count, x, y, z, i);
                    }
                }
            }
        }
    }
}

int is_face_visible(Chunk* chunk, int x, int y, int z, int dx, int dy, int dz) {
    int nx = x + dx;
    int ny = y + dy;
    int nz = z + dz;

    if (nx < 0 || nx >= CHUNK_SIZE_X ||
        ny < 0 || ny >= CHUNK_SIZE_Y ||
        nz < 0 || nz >= CHUNK_SIZE_Z) {
        return 1;
    }

    return chunk->blocks[nx][ny][nz] == 0;
}


int add_face(vec3* out, int x, int y, int z, int face) {
    vec3 base_vertices[] = {
        {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}, // 0-3: front face
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, // 4-7: back face
    };

    static const unsigned char face_indices[6][4] = {
        {4, 5, 6, 7}, // back (-Z)
        {0, 1, 2, 3}, // front (+Z)
        {4, 0, 3, 7}, // left (-X)
        {1, 5, 6, 2}, // right (+X)
        {4, 5, 1, 0}, // bottom (-Y)
        {3, 2, 6, 7}, // top (+Y)
    };

    vec3 world_vertices[8];
    for (int i = 0; i < 8; ++i) {
        glm_vec3_copy(base_vertices[i], world_vertices[i]);
        world_vertices[i][0] += x;
        world_vertices[i][1] += y;
        world_vertices[i][2] += z;
    }

    const unsigned char* idx = face_indices[face];
    glm_vec3_copy(world_vertices[idx[0]], out[0]);
    glm_vec3_copy(world_vertices[idx[1]], out[1]);
    glm_vec3_copy(world_vertices[idx[2]], out[2]);

    glm_vec3_copy(world_vertices[idx[0]], out[3]);
    glm_vec3_copy(world_vertices[idx[2]], out[4]);
    glm_vec3_copy(world_vertices[idx[3]], out[5]);

    return 6;
}